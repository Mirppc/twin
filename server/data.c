/*
 *  data.c  --  create fundamental structures:
 *              `All', screens, table for mouse/keyboard actions
 *
 *  Copyright (C) 1993-2000 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include "twin.h"
#include "methods.h"
#include "main.h"
#include "data.h"

#include "libTwkeys.h"	/* for TW_* key defines */
#include "hotkey.h"	/* for HOT_KEY */

/* First, some structures */

static udat ExecKey[STATE_MAX][MAX_KEY_CODE - ('~' - ' ')]; /* no actions associated to plain ASCII keys */
static udat ExecMouse[STATE_MAX][MAX_MOUSE_CODE];

static byte GtransUser[0x80];

/***************/

static screen *OneScreen, *TwoScreen;

static setup SetUp = {
    (dat)1,
    { (time_t)0, 500 MilliSECs },
	(udat)0x003F,
	(byte)0,	 /* Flags */
	(byte)5, (byte)3 /* DeltaXShade, DeltaYShade */
};

static selection Selection = {
    { (time_t)0, (frac_t)0 },
	(msgport *)0, (display_hw *)0,
	SEL_TEXTMAGIC,
	"",
	(uldat)0, (uldat)0,
	NULL
};

#define L 0x55
#define M 0xAA
#define H 0xFF

palette Palette[MAXCOL+1] = {
    /* the default colour table, for VGA+ colour systems */
    {0,0,0}, {0,0,M}, {0,M,0}, {0,M,M}, {M,0,0}, {M,0,M}, {M,M,0}, {M,M,M},
    {L,L,L}, {L,L,H}, {L,H,L}, {L,H,H}, {H,L,L}, {H,L,H}, {H,H,L}, {H,H,H}};

palette defaultPalette[MAXCOL+1] = {
    /* the default colour table, for VGA+ colour systems */
    {0,0,0}, {0,0,M}, {0,M,0}, {0,M,M}, {M,0,0}, {M,0,M}, {M,M,0}, {M,M,M},
    {L,L,L}, {L,L,H}, {L,H,L}, {L,H,H}, {H,L,L}, {H,L,H}, {H,H,L}, {H,H,H}};

#undef H
#undef M
#undef L

static all _All = {
    (screen *)0, (screen *)0,
	(msgport *)0, (msgport *)0, (msgport *)0,
	(mutex *)0, (mutex *)0,
	(module *)0, (module *)0,
	(fn_hook)0, (window *)0,
	(display_hw *)0, (display_hw *)0, (display_hw *)0,
	(fn_hook)0, (window *)0,
	(menu *)0,
    { (time_t)0, (uldat)0 },
	&Selection,
	&SetUp,
	
    { ExecKey[0], ExecKey[1], ExecKey[2], ExecKey[3], ExecKey[4] },
    { ExecMouse[0], ExecMouse[1], ExecMouse[2], ExecMouse[3], ExecMouse[4] },
	
    {	    /*GRAF_MAP: in the range 0x80 - 0xFF it's identical to LAT1_MAP*/
	    "\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE"
	    "\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE"
	    "\xFF\xAD\x9B\x9C\xFE\x9D\xFE\x15\x22\x43\xA6\xAE\xAA\x2D\x52\xFE"
	    "\xF8\xF1\xFD\xFE\xFE\xE6\x14\xFA\x2C\xFE\xA7\xAF\xAC\xAB\xFE\xA8"
	    "\x41\x41\x41\x41\x8E\x8F\x92\x80\x45\x90\x45\x45\x49\x49\x49\x49"
	    "\xFE\xA5\x4F\x4F\x4F\x4F\x99\x78\xE8\x55\x55\x55\x9A\x59\xFE\xE1"
	    "\x85\xA0\x83\x61\x84\x86\x91\x87\x8A\x82\x88\x89\x8D\xA1\x8C\x8B"
	    "\xFE\xA4\x95\xA2\x93\x6F\x94\xF6\xED\x97\xA3\x96\x81\x79\xFE\x98",
	    /*LAT1_MAP*/
	    "\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE"
	    "\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE\xFE"
	    "\xFF\xAD\x9B\x9C\xFE\x9D\xFE\x15\x22\x43\xA6\xAE\xAA\x2D\x52\xFE"
	    "\xF8\xF1\xFD\xFE\xFE\xE6\x14\xFA\x2C\xFE\xA7\xAF\xAC\xAB\xFE\xA8"
	    "\x41\x41\x41\x41\x8E\x8F\x92\x80\x45\x90\x45\x45\x49\x49\x49\x49"
	    "\xFE\xA5\x4F\x4F\x4F\x4F\x99\x78\xE8\x55\x55\x55\x9A\x59\xFE\xE1"
	    "\x85\xA0\x83\x61\x84\x86\x91\x87\x8A\x82\x88\x89\x8D\xA1\x8C\x8B"
	    "\xFE\xA4\x95\xA2\x93\x6F\x94\xF6\xED\x97\xA3\x96\x81\x79\xFE\x98",
	    /*USER_MAP*/
	    GtransUser
    },
	
	OV_LEFT<<LEFT | OV_MIDDLE<<MIDDLE | OV_RIGHT<<RIGHT, FALSE,
	(void (*)(void))0
};
all *All = &_All;


gadget GadgetFlag = {
    NOID, (fn_gadget *)0,
	(gadget *)0, (gadget *)0,
	(window *)0,
	COL(YELLOW,BLUE), COL(YELLOW,HIGH|BLUE), COL(HIGH|BLACK,BLUE), COL(HIGH|BLACK,BLUE),
	(udat)0,
	(udat)0, (udat)0,
	(udat)3, (udat)1,
	WINFL_USE_DEFCOL,
    {
	"[\004]",
	    "[\004]",
	    "[\004]",
	    "[\004]",
	    (byte *)0,
	    (byte *)0,
	    (byte *)0,
	    (byte *)0
    }
},
GadgetSwitch = {
    NOID, (fn_gadget *)0,
	(gadget *)0, (gadget *)0,
	(window *)0,
	COL(YELLOW,BLUE), COL(YELLOW,HIGH|BLUE), COL(HIGH|BLACK,BLUE), COL(HIGH|BLACK,BLUE),
	(udat)0,
	(udat)0, (udat)0,
	(udat)3, (udat)1,
	WINFL_USE_DEFCOL,
    {
	"(\007)",
	    "(\007)",
	    "(\007)",
	    "(\007)",
	    (byte *)0,
	    (byte *)0,
	    (byte *)0,
	    (byte *)0
    }
};

byte GadgetResize[2][2] = {
    {'�', '�'},
    {210, 226}
},
ScrollBarX[2][3] = {
    {'�', '\x11', '\x10'},
    {227, '\x11', '\x10'}
},
ScrollBarY[2][3] = {
    {'�', '\x1E', '\x1f'},
    {228, '\x1E', '\x1f'}
},
TabX[2] = {
    '�',
	211
},
TabY[2] = {
    '�',
	234
},
StdBorder[2][2][3][3] = {
    {{{'�', '�', '�'},
	{'�', ' ', '�'},
	{'�', '�', '�'}},
	{{'�', '�', '�'},
	    {'�', ' ', '�'},
	    {'�', '�', '�'}}},
    {{{212, 213, 233},
	{229, ' ', 230},
	{212, 213, 233}},
	{{212, 213, 233},
	    {231, ' ', 232},
	    {212, 213, 233}}},
};


byte Button_N = 3;
num  Button_Close = 0, Button_Back = 2;
num  Button_Pos[MAX_BUTTONS] = { 0, -3, -1, };
byte Button_Fn[MAX_BUTTONS] = { BUTTON_CLOSE, BUTTON_ROLLUP, BUTTON_BACK, };
byte Button_Shape[MAX_BUTTONS][2][2] = {
    {{'[', ']'},
	{208, 224}},
    {{'>', '<'},
	{175, 174}},
    {{18, 18},
	{209, 225}},
};
byte Button_Delta_Left = 2, Button_Delta_Right = 4;

hwcol DEFAULT_ColGadgets = COL(HIGH|YELLOW,CYAN),
DEFAULT_ColArrows = COL(HIGH|GREEN,HIGH|BLUE),
DEFAULT_ColBars = COL(WHITE,HIGH|BLUE),
DEFAULT_ColTabs = COL(HIGH|WHITE,HIGH|BLUE),
DEFAULT_ColBorder = COL(HIGH|WHITE,HIGH|BLUE),
DEFAULT_ColDisabled = COL(HIGH|BLACK,BLACK),
DEFAULT_ColSelectDisabled = COL(BLACK,HIGH|BLACK);

#if 0
font NewFont8[] = {
    { 30, "\x10\x10\x38\x38\x7c\x7c\xfe\0"},
    { 31, "\xfe\x7c\x7c\x38\x38\x10\x10\0"},
    { 42, "\0\x44\x28\xfe\x28\x44\0\0"},
    { 48, "\0\x7c\x4c\x56\x56\x66\x7e\0"},
    { 49, "\0\x8\x8\x8\x18\x18\x18\0"},
    { 50, "\0\x7e\x2\x7e\x60\x60\x7e\0"},
    { 51, "\0\x7c\x4\x7e\x6\x6\x7e\0"},
    { 52, "\0\x60\x62\x62\x7e\x2\x2\0"},
    { 53, "\0\x7c\x40\x7e\x6\x6\x7e\0"},
    { 54, "\0\x3e\x20\x7e\x62\x62\x7e\0"},
    { 55, "\0\x7c\x4\x4\x6\x6\x6\0"},
    { 56, "\0\x3c\x24\x3c\x66\x66\x7e\0"},
    { 57, "\0\x7c\x44\x7e\x6\x6\x7e\0"},
    { 65, "\0\x7c\x44\x7e\x46\x46\x46\0"},
    { 66, "\0\x3c\x22\x7c\x62\x62\x7c\0"},
    { 67, "\0\x3c\x20\x60\x60\x60\x7c\0"},
    { 68, "\0\x3c\x22\x62\x62\x62\x7c\0"},
    { 69, "\0\x3e\x20\x7e\x60\x60\x7e\0"},
    { 70, "\0\x3e\x20\x7e\x60\x60\x60\0"},
    { 71, "\0\x3e\x20\x60\x62\x62\x7e\0"},
    { 72, "\0\x44\x44\x7e\x46\x46\x46\0"},
    { 73, "\0\x10\x10\x18\x18\x18\x18\0"},
    { 74, "\0\x1c\x8\x8\xc\x2c\x3c\0"},
    { 75, "\0\x22\x24\x78\x64\x62\x62\0"},
    { 76, "\0\x20\x20\x60\x60\x62\x7e\0"},
    { 77, "\0\x7c\x54\x56\x56\x46\x46\0"},
    { 78, "\0\x22\x32\x6a\x6a\x66\x62\0"},
    { 79, "\0\x7c\x44\x46\x46\x46\x7e\0"},
    { 80, "\0\x3e\x22\x7e\x60\x60\x60\0"},
    { 81, "\0\x7c\x44\x46\x56\x4e\x7e\0"},
    { 82, "\0\x3e\x22\x7e\x64\x64\x66\0"},
    { 83, "\0\x7e\x40\x7e\x6\x46\x7e\0"},
    { 84, "\0\x7c\x10\x10\x18\x18\x18\0"},
    { 85, "\0\x44\x44\x46\x46\x46\x7e\0"},
    { 86, "\0\x22\x22\x62\x64\x68\x70\0"},
    { 87, "\0\x22\x22\x6a\x6a\x6a\x7e\0"},
    { 88, "\0\x44\x28\x10\x2c\x46\x46\0"},
    { 89, "\0\x44\x28\x10\x18\x18\x18\0"},
    { 90, "\0\x7e\x2\x1c\x60\x60\x7e\0"},
    {208, "\xff\x80\x9f\x90\x9f\x8f\x80\xff"},
    {209, "\xff\x80\x81\x83\x87\x8f\x80\xff"},
    {210, "\xff\x82\x82\x82\xfe\x80\x80\xff"},
    {211, "\0\0\xff\xff\xff\xff\0\0"},
    {212, "\xff\x80\x80\x80\x80\x80\x80\xff"},
    {213, "\xff\0\0\0\0\0\0\xff"},
    {224, "\xff\x1\xf1\x19\xf9\xf9\x1\xff"},
    {225, "\xff\x1\x1\x81\xc1\xe1\x1\xff"},
    {226, "\xff\x1\x39\x31\x29\x5\x3\xff"},
    {227, "\xff\xaa\x55\xaa\x55\xaa\x55\xff"},
    {228, "\xd5\xab\xd5\xab\xd5\xab\xd5\xab"},
    {229, "\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc"},
    {230, "\x33\x33\x33\x33\x33\x33\x33\x33"},
    {231, "\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0"},
    {232, "\x3\x3\x3\x3\x3\x3\x3\x3"},
    {233, "\xff\x1\x1\x1\x1\x1\x1\xff"},
    {234, "\x3c\x3c\x3c\x3c\x3c\x3c\x3c\x3c"},
    {251, "\0\02\06\x8C\xD8\x70\x20\0"},
    {  0, ""}
},
NewFont14[] = {
    { 16, "\xff\x1\x41\x61\x71\x79\x7d\x79\x71\x61\x41\x1\x1\xff"},
    { 17, "\xff\x80\x82\x86\x8e\x9e\xbe\x9e\x8e\x86\x82\x80\x80\xff"},
    { 30, "\xff\x1\x1\x1\x11\x11\x39\x39\x7d\x7d\xff\xff\x1\x1"},
    { 31, "\x1\x1\xff\xff\x7d\x7d\x39\x39\x11\x11\x1\x1\x1\xff"},
    {208, "\xff\x80\x80\x80\x80\x9f\x90\x9f\x8f\x80\x80\x80\x80\xff"},
    {209, "\xff\x81\x83\x87\x8f\x9f\x80\x9f\x8f\x87\x83\x81\x80\xff"},
    {210, "\xff\x82\x82\x82\x82\xfe\x80\x80\x80\x80\x80\x80\x80\xff"},
    {211, "\xff\xff\xff\0\0\0\0\0\0\0\0\xff\xff\xff"},
    {212, "\xff\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\xff"},
    {213, "\xff\0\0\0\0\0\0\0\0\0\0\0\0\xff"},
    {224, "\xff\x1\x1\x1\x1\xf1\x19\xf9\xf9\x1\x1\x1\x1\xff"},
    {225, "\xff\x1\x81\xc1\xe1\xf1\x1\xf1\xe1\xc1\x81\x1\x1\xff"},
    {226, "\xff\x1\x1\x1\x1\x1\xf1\xe1\xe1\x91\x9\x5\x1\xff"},
    {227, "\xff\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xff"},
    {228, "\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab"},
    {229, "\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc"},
    {230, "\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33"},
    {231, "\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0"},
    {232, "\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3"},
    {233, "\xff\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\xff"},
    {234, "\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3"},
    {251, "\0\0\03\03\06\06\0C\x8C\xD8\x78\x30\0\0\0"},
    {  0, ""}
},
NewFont16[] = {
    { 16, "\xff\x1\x81\xc1\xe1\xf1\xf9\xfd\xf9\xf1\xe1\xc1\x81\x1\x1\xff"},
    { 17, "\xff\x80\x81\x83\x87\x8f\x9f\xbf\x9f\x8f\x87\x83\x81\x80\x80\xff"},
    { 30, "\xff\x1\x1\x1\x1\x11\x11\x39\x39\x7d\x7d\xff\xff\x1\x1\x1"},
    { 31, "\x1\x1\x1\xff\xff\x7d\x7d\x39\x39\x11\x11\x1\x1\x1\x1\xff"},
    {208, "\xff\x80\x80\x80\x80\x80\x9f\x90\x9f\x8f\x80\x80\x80\x80\x80\xff"},
    {209, "\xff\x80\x81\x83\x87\x8f\x9f\x80\x9f\x8f\x87\x83\x81\x80\x80\xff"},
    {210, "\xff\x82\x82\x82\x82\x82\xfe\x80\x80\x80\x80\x80\x80\x80\x80\xff"},
    {211, "\xff\xff\xff\xff\0\0\0\0\0\0\0\0\xff\xff\xff\xff"},
    {212, "\xff\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\x80\xff"},
    {213, "\xff\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xff"},
    {224, "\xff\x1\x1\x1\x1\x1\xf1\x19\xf9\xf9\x1\x1\x1\x1\x1\xff"},
    {225, "\xff\x1\x1\x81\xc1\xe1\xf1\x1\xf1\xe1\xc1\x81\x1\x1\x1\xff"},
    {226, "\xff\x1\x1\x1\x1\x1\x1\x1\xf1\xe1\xe1\x91\x9\x5\x1\xff"},
    {227, "\xff\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xaa\x55\xff"},
    {228, "\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab\xd5\xab"},
    {229, "\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc"},
    {230, "\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33"},
    {231, "\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0"},
    {232, "\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3\x3"},
    {233, "\xff\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\xff"},
    {234, "\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3\xc3"},
    {251, "\0\0\0\03\03\06\06\0C\x8C\xD8\x78\x30\0\0\0\0"},
    {  0, ""}
};
#endif

byte InitData(void) {
    /*----------- Default -------------*/
    
    ExecM(STATE_DEFAULT,0)=(udat)0; 	/* Normal Format (not compressed) */
    ExecK(STATE_DEFAULT,0)=(udat)1; 	/* Compressed, parallel list format */
    
    ExecM(STATE_DEFAULT,DOWN_LEFT           )=STDEF_MOUSE_ACT_SOME;
    ExecM(STATE_DEFAULT,DOWN_RIGHT          )=STDEF_MOUSE_ACT_MENU;
    ExecM(STATE_DEFAULT,DOWN_MIDDLE         )=STDEF_MOUSE_ACT_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE|HOLD_LEFT)=STDEF_MOUSE_DRAG_SOME;
    ExecM(STATE_DEFAULT,RELEASE_LEFT        )=STWIN_BACKTO_DEF;
    ExecM(STATE_DEFAULT,RELEASE_RIGHT       )=STWIN_BACKTO_DEF;
    ExecM(STATE_DEFAULT,RELEASE_MIDDLE      )=STWIN_BACKTO_DEF;
    
    ExecM(STATE_DEFAULT,DRAG_MOUSE    | HOLD_LEFT | HOLD_RIGHT                 )=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE    | HOLD_LEFT | HOLD_RIGHT    | HOLD_MIDDLE)=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE    | HOLD_LEFT                 | HOLD_MIDDLE)=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE                | HOLD_RIGHT                 )=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE                | HOLD_RIGHT    | HOLD_MIDDLE)=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,DRAG_MOUSE                                | HOLD_MIDDLE)=STDEF_MOUSE_DRAG_NULL;
    ExecM(STATE_DEFAULT,RELEASE_LEFT              | HOLD_RIGHT                 )=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,RELEASE_LEFT              | HOLD_RIGHT    | HOLD_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,RELEASE_LEFT                              | HOLD_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,HOLD_LEFT              | RELEASE_RIGHT                 )=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,HOLD_LEFT              | RELEASE_RIGHT    | HOLD_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,                         RELEASE_RIGHT    | HOLD_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,HOLD_LEFT                              | RELEASE_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,                HOLD_LEFT | HOLD_RIGHT | RELEASE_MIDDLE)=STWIN_BACKTO_NULL;
    ExecM(STATE_DEFAULT,                            HOLD_RIGHT | RELEASE_MIDDLE)=STWIN_BACKTO_NULL;
    
#if 0
    ExecK(STATE_DEFAULT, 45)=STDEF_QUIT;			/*Alt-X      */
    ExecK(STATE_DEFAULT,106)=STDEF_CLOSE_WINDOW;		/*Alt-F3     */
    ExecK(STATE_DEFAULT, 63)=STDEF_NEXT_WINDOW;		/*F5         */
    ExecK(STATE_DEFAULT, 98)=STDEF_DRAGorRESIZE_WINDOW;	/*Ctrl-F5    */
    ExecK(STATE_DEFAULT,108)=STDEF_CENTER_WINDOW;		/*Alt-F5     */
    ExecK(STATE_DEFAULT, 64)=STDEF_NEXT_SCREEN;		/*F6         */
    ExecK(STATE_DEFAULT, 99)=STDEF_DRAGorRESIZE_SCREEN;	/*Ctrl-F6    */
#endif
    
    ExecK(STATE_DEFAULT,1)=HOT_KEY;		/* this sets the Menu HotKey; edit hotkey.h to change it */
    ExecK(STATE_DEFAULT,2)=STDEF_ACT_MENU;
    
    /*----------- Window -----------*/
     
    ExecM(STATE_WINDOW,0)=(udat)0; 	/* Normal Format (not compressed) */
    ExecK(STATE_WINDOW,0)=(udat)0; 	/* Normal Format (not compressed) */
   
    ExecM(STATE_WINDOW,DOWN_LEFT                              )=STWIN_MOUSE_ACT_SOME;
    ExecM(STATE_WINDOW,DRAG_MOUSE | HOLD_LEFT                 )=STWIN_MOUSE_DRAG_SOME;
    ExecM(STATE_WINDOW,RELEASE_LEFT                           )=STWIN_BACKTO_DEF;
    ExecM(STATE_WINDOW,RELEASE_LEFT | HOLD_RIGHT              )=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_WINDOW,RELEASE_LEFT | HOLD_RIGHT | HOLD_MIDDLE)=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_WINDOW,RELEASE_LEFT              | HOLD_MIDDLE)=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_WINDOW,HOLD_LEFT | RELEASE_RIGHT              )=STWIN_MOUSE_DRAG_SOME;
    ExecM(STATE_WINDOW,HOLD_LEFT | RELEASE_RIGHT | HOLD_MIDDLE)=STWIN_MOUSE_DRAG_SOME;
    ExecM(STATE_WINDOW,HOLD_LEFT | HOLD_RIGHT | RELEASE_MIDDLE)=STWIN_MOUSE_DRAG_SOME;
    ExecM(STATE_WINDOW,HOLD_LEFT              | RELEASE_MIDDLE)=STWIN_MOUSE_DRAG_SOME;

    ExecK(STATE_WINDOW,TW_Return)=STWIN_BACKTO_DEF;
    ExecK(STATE_WINDOW,TW_Escape)=STWIN_BACKTO_DEF;
    ExecK(STATE_WINDOW,TW_Left  )=STWIN_Xn_DRAG;
    ExecK(STATE_WINDOW,TW_Right )=STWIN_Xp_DRAG;
    ExecK(STATE_WINDOW,TW_Up    )=STWIN_Yn_DRAG;
    ExecK(STATE_WINDOW,TW_Down  )=STWIN_Yp_DRAG;
    ExecK(STATE_WINDOW,TW_Insert)=STWIN_X_INCR;
    ExecK(STATE_WINDOW,TW_Delete)=STWIN_X_DECR;
    ExecK(STATE_WINDOW,TW_Prior )=STWIN_Y_DECR;
    ExecK(STATE_WINDOW,TW_Next  )=STWIN_Y_INCR;

    /*----------- Scroll. This is keyboard only. -----------*/
     
    ExecM(STATE_SCROLL,0)=(udat)0; 	/* Normal Format (not compressed) */
    ExecK(STATE_SCROLL,0)=(udat)0; 	/* Normal Format (not compressed) */
   
    ExecM(STATE_SCROLL,DOWN_LEFT                              )=STWIN_MOUSE_ACT_SOME;
    ExecM(STATE_SCROLL,DOWN_MIDDLE			      )=STDEF_MOUSE_ACT_NULL;
    ExecM(STATE_SCROLL,DOWN_RIGHT			      )=STDEF_MOUSE_ACT_MENU;
    ExecM(STATE_SCROLL,RELEASE_LEFT                           )=STWIN_BACKTO_DEF;
    ExecM(STATE_SCROLL,RELEASE_MIDDLE			      )=STWIN_BACKTO_DEF;
    ExecM(STATE_SCROLL,RELEASE_RIGHT			      )=STWIN_BACKTO_DEF;

    ExecK(STATE_SCROLL,TW_Return)=STWIN_BACKTO_DEF;
    ExecK(STATE_SCROLL,TW_Escape)=STWIN_BACKTO_DEF;
    ExecK(STATE_SCROLL,TW_Left  )=SCROLL_Xn_CHAR;
    ExecK(STATE_SCROLL,TW_Right )=SCROLL_Xp_CHAR;
    ExecK(STATE_SCROLL,TW_Up    )=SCROLL_Yn_CHAR;
    ExecK(STATE_SCROLL,TW_Down  )=SCROLL_Yp_CHAR;
    ExecK(STATE_SCROLL,TW_Insert)=SCROLL_Xp_PAGE;
    ExecK(STATE_SCROLL,TW_Delete)=SCROLL_Xn_PAGE;
    ExecK(STATE_SCROLL,TW_Prior )=SCROLL_Yn_PAGE;
    ExecK(STATE_SCROLL,TW_Next  )=SCROLL_Yp_PAGE;

    /*------------ Menu --------------*/
    
    ExecM(STATE_MENU,0)=(udat)0; 	/* Normal Format (not compressed) */
    ExecK(STATE_MENU,0)=(udat)0; 	/* Normal Format (not compressed) */
    
    ExecM(STATE_MENU,               DOWN_RIGHT                 )=STMENU_MOUSE_ACT_MENU;
    ExecM(STATE_MENU,DRAG_MOUSE   | HOLD_RIGHT                 )=STMENU_MOUSE_DRAG;
    ExecM(STATE_MENU,            RELEASE_RIGHT                 )=STMENU_ACT_BACKTO_DEF;
    ExecM(STATE_MENU,RELEASE_LEFT | HOLD_RIGHT                 )=STMENU_MOUSE_DRAG;
    ExecM(STATE_MENU,RELEASE_LEFT | HOLD_RIGHT | HOLD_MIDDLE   )=STMENU_MOUSE_DRAG;
    ExecM(STATE_MENU,HOLD_LEFT    | HOLD_RIGHT | RELEASE_MIDDLE)=STMENU_MOUSE_DRAG;
    ExecM(STATE_MENU,               HOLD_RIGHT | RELEASE_MIDDLE)=STMENU_MOUSE_DRAG;
    ExecM(STATE_MENU,HOLD_LEFT | RELEASE_RIGHT                 )=STMENU_BACKTO_DEF_FREEZE;
    ExecM(STATE_MENU,HOLD_LEFT | RELEASE_RIGHT | HOLD_MIDDLE   )=STMENU_BACKTO_DEF_FREEZE;
    ExecM(STATE_MENU,            RELEASE_RIGHT | HOLD_MIDDLE   )=STMENU_BACKTO_DEF_FREEZE;
    
#if 0
    ExecK(STATE_MENU, 45)=STMENU_QUIT;			/*Alt-X      */
    ExecK(STATE_MENU, 98)=STMENU_DRAGorRESIZE_WINDOW;	/*Ctrl-F5    */
#endif
    ExecK(STATE_MENU,TW_Return)=STMENU_ACT_BACKTO_DEF;
    ExecK(STATE_MENU,TW_Escape)=STMENU_BACKTO_DEF;
    ExecK(STATE_MENU,TW_Left  )=STMENU_PREV_ITEM;
    ExecK(STATE_MENU,TW_Right )=STMENU_NEXT_ITEM;
    ExecK(STATE_MENU,TW_Up    )=STMENU_PREV_ROW;
    ExecK(STATE_MENU,TW_Down  )=STMENU_NEXT_ROW;
    
    /*---------- Screen ---------*/
    
    ExecM(STATE_SCREEN,0)=(udat)0; 	/* Normal Format (not compressed) */
    ExecK(STATE_SCREEN,0)=(udat)0; 	/* Normal Format (not compressed) */
    
    ExecM(STATE_SCREEN,DOWN_LEFT                              )=STDEF_MOUSE_ACT_SOME;
    ExecM(STATE_SCREEN,DRAG_MOUSE | HOLD_LEFT                 )=STDEF_MOUSE_DRAG_SOME;
    ExecM(STATE_SCREEN,RELEASE_LEFT                           )=STWIN_BACKTO_DEF;
    ExecM(STATE_SCREEN,RELEASE_LEFT | HOLD_RIGHT              )=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_SCREEN,RELEASE_LEFT | HOLD_RIGHT | HOLD_MIDDLE)=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_SCREEN,RELEASE_LEFT              | HOLD_MIDDLE)=STWIN_BACKTO_DEF_FREEZE;
    ExecM(STATE_SCREEN,HOLD_LEFT | RELEASE_RIGHT              )=STDEF_MOUSE_DRAG_SOME;
    ExecM(STATE_SCREEN,HOLD_LEFT | RELEASE_RIGHT | HOLD_MIDDLE)=STDEF_MOUSE_DRAG_SOME;
    ExecM(STATE_SCREEN,HOLD_LEFT | HOLD_RIGHT | RELEASE_MIDDLE)=STDEF_MOUSE_DRAG_SOME;
    ExecM(STATE_SCREEN,HOLD_LEFT              | RELEASE_MIDDLE)=STDEF_MOUSE_DRAG_SOME;
    
    ExecK(STATE_SCREEN,TW_Return)=STWIN_BACKTO_DEF;
    ExecK(STATE_SCREEN,TW_Escape)=STWIN_BACKTO_DEF;
    ExecK(STATE_SCREEN,TW_Left  )=SCREEN_Xn_SCROLL;
    ExecK(STATE_SCREEN,TW_Right )=SCREEN_Xp_SCROLL;
    ExecK(STATE_SCREEN,TW_Up    )=SCREEN_Yn_SCROLL;
    ExecK(STATE_SCREEN,TW_Down  )=SCREEN_Yp_SCROLL;
    ExecK(STATE_SCREEN,TW_Prior )=SCREEN_Ln_SCROLL;
    ExecK(STATE_SCREEN,TW_Next  )=SCREEN_Lp_SCROLL;

    GadgetFlag.Fn = GadgetSwitch.Fn = FnGadget;
    
    if ((OneScreen = Do(CreateSimple,Screen)(FnScreen, HWATTR(COL(HIGH|BLACK,BLUE),'�')))) {
	
#define a HWATTR(COL(HIGH|BLUE,BLUE),'�')
#define b HWATTR(COL(HIGH|BLUE,BLUE),' ')
#define c HWATTR(COL(HIGH|BLUE,BLUE),'�')
	hwattr attr[8] = {
	    b,b,a,c,
	    c,a,b,b,
	};
#undef a
#undef b
	if ((TwoScreen = Do(Create,Screen)(FnScreen, 4, 2, attr))) {
	
	    InsertLast(Screen, OneScreen, All);
	    InsertLast(Screen, TwoScreen, All);
	    
	    return TRUE;
	}
    }
    fprintf(stderr, "twin: Out of memory!\n");
    return FALSE;
}

