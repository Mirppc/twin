#ifndef _TW_DATA_H
#define _TW_DATA_H

extern CONST byte *conf_destdir_lib_twin;
extern CONST byte *conf_destdir_lib_twin_modules_;

extern palette Palette[MAXCOL+1], defaultPalette[MAXCOL+1];

extern all All;

typedef struct keylist keylist;
struct keylist {
    byte *name;
    udat key;
    byte len;
    byte *seq;
};

extern keylist TW_KeyList[];

extern gadget GadgetFlag, GadgetSwitch;

extern hwfont GadgetResize[2],
    ScrollBarX[3],
    ScrollBarY[3],
    TabX,
    TabY,
    StdBorder[2][9],
    Screen_Back[2];

extern hwcol DEFAULT_ColGadgets, DEFAULT_ColArrows, DEFAULT_ColBars, DEFAULT_ColTabs,
	DEFAULT_ColBorder, DEFAULT_ColDisabled, DEFAULT_ColSelectDisabled;

byte InitData(void);


#endif /* _TW_DATA_H */
