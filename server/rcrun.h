#ifndef _TW_RCRUN_H
#define _TW_RCRUN_H

#define GLOBAL_MAX 7
extern node Globals[GLOBAL_MAX];
extern byte GlobalsAreStatic;

#define CallList	Globals[0]
#define FuncList	Globals[1]
#define MenuList	Globals[2]
#define ScreenList	Globals[3]
#define BorderList	Globals[4]
#define KeyList		Globals[5]
#define MouseList	Globals[6]

extern node *MenuBinds; /* array of pointers to nodes inside MenuList */
extern udat MenuBindsMax;

byte InitRC(void);
void QuitRC(void);

node LookupNodeName(str name, node head);

hwfont *RCFindBorderPattern(window W, byte Border);

byte RC_VM(timevalue *t);
byte RC_VMQueue(CONST wm_ctx *C);


#endif /* _TW_RCRUN_H */

