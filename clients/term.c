/*
 *  term.c  --  create and manage multiple terminal emulator windows on twin
 *
 *  Copyright (C) 1999-2000 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

#include <libTw.h>
#include "version.h"

#include "pty.h"

#define COD_QUIT      (udat)1
#define COD_ASKCLOSE  (udat)2
#define COD_SPAWN     (udat)3
#define COD_CENTER    (udat)4
#define COD_ZOOM      (udat)5
#define COD_MAXZOOM   (udat)6

static tscreen Term_Screen;
static tmsgport Term_MsgPort;
static tmenu Term_Menu;
static uldat WinN;

fd_set save_rfds;
uldat max_fds;
int twin_fd;


/* 5. remote fds handling */
typedef struct {
    int Fd;
    twindow Window;
    pid_t Pid;
} fdlist;

static fdlist *FdList;
static uldat FdSize, FdTop, FdBottom;
#define LS	FdList[Slot]


/* functions */

static uldat FdListGrow(void) {
    uldat oldsize, size;
    fdlist *newFdList;
    
    if ((oldsize = FdSize) == MAXULDAT)
	return NOSLOT;
    
    if ((size = oldsize < 64 ? 96 : oldsize + (oldsize>>1)) < oldsize)
	size = MAXULDAT;
    
    if (!(newFdList = (fdlist *)TwReAllocMem(FdList, size*sizeof(fdlist))))
	return NOSLOT;
    
    for (FdSize = oldsize+1; FdSize<size; FdSize++)
	newFdList[FdSize].Fd = NOFD;
    
    FdList = newFdList;
    
    return oldsize;
}

INLINE uldat FdListGet(void) {
    if (FdBottom < FdSize)
	return FdBottom;
    return FdListGrow();
}

static uldat Slot_Window(twindow Window) {
    uldat Slot;
    for (Slot = 0; Slot < FdTop; Slot++) {
	if (LS.Fd != -1 && LS.Window == Window)
	    return Slot;
    }
    return NOSLOT;
}

INLINE int Fd_Slot(uldat Slot) {
    if (Slot < FdTop)
	return LS.Fd;
    return NOFD;
}

static uldat RegisterRemote(int Fd, twindow Window, pid_t Pid) {
    uldat Slot, j;
    
    if ((Slot = FdListGet()) == NOSLOT)
	return Slot;
    
    LS.Fd = Fd;
    LS.Window = Window;
    LS.Pid = Pid;
    
    if (FdTop <= Slot)
	FdTop = Slot + 1;
    for (j = FdBottom; j < FdTop; j++)
	if (FdList[j].Fd == NOFD)
	    break;
    FdBottom = j;

    FD_SET(Fd, &save_rfds);
    if (max_fds < Fd)
	max_fds = Fd;
    
    return Slot;
}

/* UnRegister a Fd and related stuff given a slot number */
static void UnRegisterRemote(uldat Slot) {
    int i;
    uldat j;
    
    if (Slot < FdTop && LS.Fd != NOFD) {
	FD_CLR(LS.Fd, &save_rfds);
	LS.Fd = NOFD;
	
	for (i=max_fds; i>=0; i--)
	    if (FD_ISSET(i, &save_rfds))
		break;
	max_fds = i;
	
	if (FdBottom > Slot)
	    FdBottom = Slot;
	for (j = FdTop; j > FdBottom; j--)
	    if (FdList[j].Fd != NOFD)
		break;
	FdTop = (j == FdBottom) ? j : j + 1;
    }
}


void Resize(uldat Slot, udat X, udat Y) {
    struct winsize wsiz;
    if (Slot < FdTop && LS.Fd != NOFD) {
	wsiz.ws_col = X;
	wsiz.ws_row = Y;
	wsiz.ws_xpixel = 0;
	wsiz.ws_ypixel = 0;
    
	if (ioctl(LS.Fd, TIOCSWINSZ, &wsiz) == 0)
	    kill(-LS.Pid, SIGWINCH);
    }
}



static char **args;
static char *title = "Twin Term";

static twindow newTermWindow(void) {
    uldat len = strlen(title);
    twindow Window = TwCreateWindow
	(len, title, NULL,
	 Term_Menu, COL(WHITE,BLACK), TW_LINECURSOR,
	 TW_WINDOW_WANT_KEYS|TW_WINDOW_WANT_CHANGES|TW_WINDOW_DRAG|TW_WINDOW_RESIZE|TW_WINDOW_Y_BAR|TW_WINDOW_CLOSE,
	 TW_WINFL_CURSOR_ON|TW_WINFL_USECONTENTS,
	 (udat)82, (udat)27, (uldat)200);

    if (Window != NOID) {
	TwSetColorsWindow
	    (Window, 0x1FF, COL(HIGH|YELLOW,CYAN), COL(HIGH|GREEN,HIGH|BLUE), COL(WHITE,HIGH|BLUE),
	     COL(HIGH|WHITE,HIGH|BLUE), COL(HIGH|WHITE,HIGH|BLUE),
	     COL(WHITE,BLACK), COL(WHITE,HIGH|BLACK), COL(HIGH|BLACK,BLACK), COL(BLACK,HIGH|BLACK));
	
	TwConfigureWindow(Window, 0xF<<2, 0, 0, (udat)7, (udat)3, MAXUDAT, MAXUDAT);
    }
    return Window;
}

static byte OpenTerm(void) {
    twindow Window;
    int Fd;
    pid_t Pid;
    uldat Slot;
    
    if ((Window = newTermWindow())) {
	if ((Fd = Spawn(Window, &Pid, args)) != NOFD) {
	    if ((Slot = RegisterRemote(Fd, Window, Pid)) != NOSLOT) {
		Resize(Slot, 80, 25);
		TwMapWindow(Window, Term_Screen);
		WinN++;
		return TRUE;
	    }
	    close(Fd);
	}
	TwDeleteWindow(Window);
    }
    return FALSE;
}

static void CloseTerm(uldat Slot) {
    close(LS.Fd);
    TwDeleteWindow(LS.Window);
    UnRegisterRemote(Slot);
    WinN--;
}

static void SignalChild(int n) {
    while (wait3((int *)0, WNOHANG, (struct rusage *)0) > 0)
	;
    signal(SIGCHLD, SignalChild);
}

static byte InitTerm(void) {
    twindow Window;
    
    signal(SIGCHLD, SignalChild);
    
    if (TwOpen(NULL)) {
	if ((Term_MsgPort=TwCreateMsgPort
	     (9, "Twin Term", (uldat)0, (udat)0, (byte)0)) &&
	    (Term_Menu=TwCreateMenu
	     (Term_MsgPort,
	      COL(BLACK,WHITE), COL(BLACK,GREEN), COL(HIGH|BLACK,WHITE), COL(HIGH|BLACK,BLACK),
	      COL(RED,WHITE), COL(RED,GREEN), (byte)0)) &&
	    (TwInfo4Menu(Term_Menu, TW_ROW_ACTIVE, (uldat)18, " Remote Twin Term ", "ptpppppptpppptpppp"),

	     (Window=TwWin4Menu(Term_Menu))) &&
	    (TwRow4Menu(Window, COD_SPAWN, TW_ROW_ACTIVE, 10, " New Term "),
	     TwRow4Menu(Window, COD_QUIT,  FALSE,       6, " Exit "),
	     TwItem4Menu(Term_Menu, Window, TRUE, 6, " File ")) &&
	    
	    TwItem4MenuCommon(Term_Menu)) {
	    
	    Term_Screen = TwFirstScreen();
	
	    if (OpenTerm())
		return TRUE;
	}
	TwClose();
    } while(0);

    if (TwErrno)
	fprintf(stderr, "twterm: libTw error: %s\n", TwStrError(TwErrno));

    return FALSE;
}

static void TwinTermH(void) {
    tmsg Msg;
    tevent_any Event;
    udat Code/*, Repeat*/;
    twindow Win;
    uldat Slot;
    int Fd;
    
    while ((Msg=TwReadMsg(FALSE))) {
	
	Event=&Msg->Event;
	Win = Event->EventCommon.Window;
	Slot = Slot_Window(Win);
	Fd = Fd_Slot(Slot);
	
	if (Msg->Type==TW_MSG_WINDOW_KEY) {
	    /* send keypresses */
	    write(Fd, Event->EventKeyboard.AsciiSeq, Event->EventKeyboard.SeqLen);
	} else if (Msg->Type==TW_MSG_SELECTION) {
	    /*
	     * send Msg->Event.EventCommon.Window as ReqPrivate field,
	     * so that we will get it back in TW_MSG_SELECTIONNOTIFY message
	     * without having to store it manually
	     */
	    TwRequestSelection(TwGetOwnerSelection(), Win);
	    
	} else if (Msg->Type==TW_MSG_SELECTIONNOTIFY) {

	    Win = Event->EventSelectionNotify.ReqPrivate;
	    Slot = Slot_Window(Win);
	    Fd = Fd_Slot(Slot);

	    /* react as for keypresses */
	    write(Fd, Event->EventSelectionNotify.Data, Event->EventSelectionNotify.Len);

	} else if (Msg->Type==TW_MSG_WINDOW_MOUSE) {
	    /* send mouse movements keypresses */
	    byte len, buf[10] = "\033[?M";
	    udat x, y;

	    Code=Event->EventMouse.Code;

	    x = Event->EventMouse.X;
	    y = Event->EventMouse.Y;
#if 0		    
	    /* classic xterm-style reporting */
	    buf[2] = 'M';
	    if (isPRESS(Code)) switch (Code & PRESS_ANY) {
	      case PRESS_LEFT: buf[3] = ' '; break;
	      case PRESS_MIDDLE: buf[3] = '!'; break;
	      case PRESS_RIGHT: buf[3] = '\"'; break;
	    }
	    else if (isRELEASE(Code))
		buf[3] = '#';
	    else {
		continue;
	    }
	    buf[4] = '!' + x;
	    buf[5] = '!' + y;
	    len = 6;
#else /* !0 */
	    /* new-style reporting */
	    buf[2] = '5';
	    buf[3] = 'M';
	    buf[4] = ' ' + (Code & HOLD_ANY);
	    buf[5] = '!' + (x & 0x7f);
	    buf[6] = '!' + (x >> 7);
	    buf[7] = '!' + (y & 0x7f);
	    buf[8] = '!' + (y >> 7);
	    len = 9;
#endif
	    write(Fd, buf, len);
	} else if (Msg->Type==TW_MSG_WINDOW_GADGET) {
	    if (!Event->EventGadget.Code)
		/* 0 == Close Code */
		CloseTerm(Slot);
	} else if (Msg->Type==TW_MSG_MENU_ROW) {
	    if (Event->EventMenu.Menu==Term_Menu) {
		Code=Event->EventMenu.Code;
		switch (Code) {
		  case COD_SPAWN:
		    OpenTerm();
		    break;
		  case COD_ASKCLOSE:
		    CloseTerm(Slot);
		    break;
		  case COD_CENTER:
		  case COD_ZOOM:
		  case COD_MAXZOOM:
		    /* can't do that remotely */
		  default:
		    break;
		}
	    }
	} else if (Msg->Type==TW_MSG_WINDOW_CHANGE) {
	    Resize(Slot, Event->EventWindow.XWidth - 2, Event->EventWindow.YWidth - 2);
	}
    }
}

static void TwinTermIO(int Slot) {
    static byte buf[BIGBUFF];
    uldat got = 0, chunk = 0;
    
    do {
	/*
	 * BIGBUFF - 1 to avoid silly windows...
	 * linux ttys buffer up to 4095 bytes.
	 */
	chunk = read(LS.Fd, buf + got, BIGBUFF - 1 - got);
    } while (chunk && chunk != (uldat)-1 && (got += chunk) < BIGBUFF - 1);
    
    if (got)
	TwWriteAsciiWindow(LS.Window, got, buf);
    else if (chunk == (uldat)-1 && errno != EINTR && errno != EAGAIN)
	/* something bad happened to our child :( */
	CloseTerm(Slot);
}

static void Usage(char *name) {
    fprintf(stderr, "Usage: %s [OPTIONS]\n"
	    "Currently known options: \n"
	    " -h, -help               display this help and exit\n"
	    " -V, -version            output version information and exit\n"
	    " -t <title>              set window title\n"
	    " -e <command>            run <command> instead of user's shell\n"
	    "                         (must be last option)\n", name);
}

static void ShowVersion(void) {
    fputs("twterm " TWIN_VERSION_STR "\n", stdout);
}

int main(int argc, char *argv[]) {
    fd_set fds;
    int num_fds;
    uldat Slot;
    struct timeval zero = {0, 0}, *pt;
    char *t, *name = argv[0], *shell[3];
    
    FD_ZERO(&save_rfds);

    argv++, argc--;
    
    while (argc) {
	if (!strcmp(*argv, "-h") || !strcmp(*argv, "-help")) {
	    Usage(name);
	    return 0;
	} else if (!strcmp(*argv, "-V") || !strcmp(*argv, "-version")) {
	    ShowVersion();
	    return 0;
	} else if (argc > 1 && !strcmp(*argv, "-t")) {
	    title = *++argv;
	    argc--;
	} else if (argc > 1 && !strcmp(*argv, "-e")) {
	    args = argv;
	    args[0] = args[1];
	    break;
	} else {
	    fprintf(stderr, "%s: argument `%s' not recognized\n"
		    "\ttry `%s -help' for usage summary.\n", name, *argv, name);
	    return 1;
	}
	argv++;
	argc--;
    }
    
    if (!args) {
	if ((shell[0] = getenv("SHELL")) &&
	    (shell[0] = strdup(shell[0])) &&
	    (shell[1] = (t = strrchr(shell[0], '/'))
	     ? strdup(t) : strdup(shell[0]))) {
	    
	    if (shell[1][0] == '/')
		shell[1][0] = '-';
	    shell[2] = NULL;
	    args = shell;
	} else
	    return 1;
    }

    if (!InitTerm())
	return 0;

    twin_fd = TwConnectionFd();
    FD_SET(twin_fd, &save_rfds);
    if (max_fds < twin_fd)
	max_fds = twin_fd;

    while (WinN) {
	/* bail out if something goes *really* wrong */
	if (!TwSync())
	    break;

	if (TwPeekMsg())
	    /* some Msg is available, don't sleep */
	    pt = &zero;
	else
	    pt = NULL;
	
	fds = save_rfds;
	do {
	    num_fds = select(max_fds+1, &fds, NULL, NULL, pt);
	} while (num_fds < 0 && errno == EINTR);
	if (num_fds < 0) {
	    /* panic! */
	    TwClose();
	    return 1;
	}
	
	TwinTermH();

	/*
	 * We cannot rely on FD_ISSET(twin_fd, &fds) to call TwinTermH(),
	 * as Msgs may have been already received through the socket
	 * and sitting in a local queue
	 */
	if (num_fds && FD_ISSET(twin_fd, &fds))
	    num_fds--;
	
	for (Slot = 0; num_fds && Slot < FdTop; Slot++) {
	    if (LS.Fd != NOFD && FD_ISSET(LS.Fd, &fds))
		TwinTermIO(Slot), num_fds--;
	}
    }
    if (TwErrno) {
	fprintf(stderr, "twterm: libTw error: %s\n", TwStrError(TwErrno));
	TwClose();
	return 1;
    }
    return 0;
}
