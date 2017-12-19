/*
 *  Twkeys.h  --  keycode enum for libTw keyboard events
 *
 *  Copyright (C) 2000,2016 by Massimiliano Ghilardi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 */

#ifndef _TW_KEYS_H
#define _TW_KEYS_H

enum Twkey_e {

    TW_Tab                 = 0x09,
    TW_Linefeed            = 0x0A,
    TW_Return              = 0x0D,
    TW_Escape              = 0x1B,
    TW_space               = 0x20,

/* no enums for 0x21...0x7E chars: they are plain ASCII! */

/* Latin 1 */

    TW_nobreakspace        = 0xA0,
    TW_exclamdown          = 0xA1,
    TW_cent                = 0xA2,
    TW_sterling            = 0xA3,
    TW_currency            = 0xA4,
    TW_yen                 = 0xA5,
    TW_brokenbar           = 0xA6,
    TW_section             = 0xA7,
    TW_diaeresis           = 0xA8,
    TW_copyright           = 0xA9,
    TW_ordfeminine         = 0xAA,
    TW_guillemotleft       = 0xAB,   /* left angle quotation mark */
    TW_notsign             = 0xAC,
    TW_hyphen              = 0xAD,
    TW_registered          = 0xAE,
    TW_macron              = 0xAF,
    TW_degree              = 0xB0,
    TW_plusminus           = 0xB1,
    TW_twosuperior         = 0xB2,
    TW_threesuperior       = 0xB3,
    TW_acute               = 0xB4,
    TW_mu                  = 0xB5,
    TW_paragraph           = 0xB6,
    TW_periodcentered      = 0xB7,
    TW_cedilla             = 0xB8,
    TW_onesuperior         = 0xB9,
    TW_masculine           = 0xBA,
    TW_guillemotright      = 0xBB    /* right angle quotation mark */,
    TW_onequarter          = 0xBC,
    TW_onehalf             = 0xBD,
    TW_threequarters       = 0xBE,
    TW_questiondown        = 0xBF,
    TW_Agrave              = 0xC0,
    TW_Aacute              = 0xC1,
    TW_Acircumflex         = 0xC2,
    TW_Atilde              = 0xC3,
    TW_Adiaeresis          = 0xC4,
    TW_Aring               = 0xC5,
    TW_AE                  = 0xC6,
    TW_Ccedilla            = 0xC7,
    TW_Egrave              = 0xC8,
    TW_Eacute              = 0xC9,
    TW_Ecircumflex         = 0xCA,
    TW_Ediaeresis          = 0xCB,
    TW_Igrave              = 0xCC,
    TW_Iacute              = 0xCD,
    TW_Icircumflex         = 0xCE,
    TW_Idiaeresis          = 0xCF,
    TW_ETH                 = 0xD0,
    TW_Ntilde              = 0xD1,
    TW_Ograve              = 0xD2,
    TW_Oacute              = 0xD3,
    TW_Ocircumflex         = 0xD4,
    TW_Otilde              = 0xdD,
    TW_Odiaeresis          = 0xD6,
    TW_multiply            = 0xD7,
    TW_Ooblique            = 0xD8,
    TW_Ugrave              = 0xD9,
    TW_Uacute              = 0xDA,
    TW_Ucircumflex         = 0xDB,
    TW_Udiaeresis          = 0xDC,
    TW_Yacute              = 0xDD,
    TW_THORN               = 0xDE,
    TW_ssharp              = 0xDF,
    TW_agrave              = 0xE0,
    TW_aacute              = 0xE1,
    TW_acircumflex         = 0xE2,
    TW_atilde              = 0xE3,
    TW_adiaeresis          = 0xE4,
    TW_aring               = 0xE5,
    TW_ae                  = 0xE6,
    TW_ccedilla            = 0xE7,
    TW_egrave              = 0xE8,
    TW_eacute              = 0xE9,
    TW_ecircumflex         = 0xEA,
    TW_ediaeresis          = 0xEB,
    TW_igrave              = 0xEC,
    TW_iacute              = 0xED,
    TW_icircumflex         = 0xEE,
    TW_idiaeresis          = 0xEF,
    TW_eth                 = 0xF0,
    TW_ntilde              = 0xF1,
    TW_ograve              = 0xF2,
    TW_oacute              = 0xF3,
    TW_ocircumflex         = 0xF4,
    TW_otilde              = 0xF5,
    TW_odiaeresis          = 0xF6,
    TW_division            = 0xF7,
    TW_oslash              = 0xF8,
    TW_ugrave              = 0xF9,
    TW_uacute              = 0xFA,
    TW_ucircumflex         = 0xFB,
    TW_udiaeresis          = 0xFC,
    TW_yacute              = 0xFD,
    TW_thorn               = 0xFE,
    TW_ydiaeresis          = 0xFF,

/* end Latin 1 */


    TW_BackSpace           = 0x7F,

    TW_Home                = 0x100,
    TW_Left                = 0x101,
    TW_Up                  = 0x102,
    TW_Right               = 0x103,
    TW_Down                = 0x104,
    TW_Prior               = 0x105,
    TW_Page_Up             = TW_Prior,
    TW_Next                = 0x106,
    TW_Page_Down           = TW_Next,
    TW_End                 = 0x107,
    TW_Begin               = 0x108,
    TW_Insert              = 0x109,
    TW_Delete              = 0x10A,

    TW_Pause               = 0x10B,
    TW_Scroll_Lock         = 0x10C,
    TW_Sys_Req             = 0x10D,
    TW_Clear               = 0x10E,

    TW_Select              = 0x110,
    TW_Print               = 0x111,
    TW_Execute             = 0x112,
    TW_Undo                = 0x115,
    TW_Redo                = 0x116,
    TW_Menu                = 0x117,
    TW_Find                = 0x118,
    TW_Cancel              = 0x119,
    TW_Help                = 0x11A,
    TW_Break               = 0x11B,
    TW_Caps_Lock           = 0x11C,
    TW_Mode_switch         = TW_Caps_Lock,
    TW_Shift_Lock          = 0x11D,
    TW_Num_Lock            = 0x11E,

    TW_KP_Begin            = 0x11F,
    TW_KP_Home             = 0x120,
    TW_KP_Left             = 0x121,
    TW_KP_Up               = 0x122,
    TW_KP_Right            = 0x123,
    TW_KP_Down             = 0x124,
    TW_KP_Prior            = 0x125,
    TW_KP_Page_Up          = TW_KP_Prior,
    TW_KP_Next             = 0x126,
    TW_KP_Page_Down        = TW_KP_Next,
    TW_KP_End              = 0x127,
    TW_KP_Insert           = 0x128,
    TW_KP_Delete           = 0x129,
    TW_KP_Multiply         = 0x12A,
    TW_KP_Add              = 0x12B,
    TW_KP_Separator        = 0x12C,
    TW_KP_Subtract         = 0x12D,
    TW_KP_Decimal          = 0x12E,
    TW_KP_Divide           = 0x12F,

    TW_KP_Equal            = 0x130,
    TW_KP_F1               = 0x131,
    TW_KP_F2               = 0x132,
    TW_KP_F3               = 0x133,
    TW_KP_F4               = 0x134,

    TW_KP_Space            = 0x135,
    TW_KP_Tab              = 0x136,
    TW_KP_Enter            = 0x137,

    TW_KP_0                = 0x140,
    TW_KP_1                = 0x141,
    TW_KP_2                = 0x142,
    TW_KP_3                = 0x143,
    TW_KP_4                = 0x144,
    TW_KP_5                = 0x145,
    TW_KP_6                = 0x146,
    TW_KP_7                = 0x147,
    TW_KP_8                = 0x148,
    TW_KP_9                = 0x149,

    TW_F1                  = 0x150,
    TW_F2                  = 0x151,
    TW_F3                  = 0x152,
    TW_F4                  = 0x153,
    TW_F5                  = 0x154,
    TW_F6                  = 0x155,
    TW_F7                  = 0x156,
    TW_F8                  = 0x157,
    TW_F9                  = 0x158,
    TW_F10                 = 0x159,
    TW_F11                 = 0x15A,
    TW_F12                 = 0x15B,
    TW_F13                 = 0x15C,
    TW_F14                 = 0x15D,
    TW_F15                 = 0x15E,
    TW_F16                 = 0x15F,
    TW_F17                 = 0x160,
    TW_F18                 = 0x161,
    TW_F19                 = 0x162,
    TW_F20                 = 0x163,

    TW_Shift_L             = 0x164,
    TW_Shift_R             = 0x165,
    TW_Control_L           = 0x166,
    TW_Control_R           = 0x167,
    TW_Meta_L              = 0x168,
    TW_Meta_R              = 0x169,
    TW_Alt_L               = 0x16A,
    TW_Alt_R               = 0x16B,
    TW_Alt                 = TW_Alt_L,
    TW_AltGr               = TW_Alt_R,
    TW_Super_L             = 0x16C,
    TW_Super_R             = 0x16D,
    TW_Hyper_L             = 0x16E,
    TW_Hyper_R             = 0x16F,

    TW_EcuSign             = 0x170,
    TW_ColonSign           = 0x171,
    TW_CruzeiroSign        = 0x172,
    TW_FFrancSign          = 0x173,
    TW_LiraSign            = 0x174,
    TW_MillSign            = 0x175,
    TW_NairaSign           = 0x176,
    TW_PesetaSign          = 0x177,
    TW_RupeeSign           = 0x178,
    TW_WonSign             = 0x179,
    TW_NewSheqelSign       = 0x17A,
    TW_DongSign            = 0x17B,
    TW_EuroSign            = 0x17C,

/* a dummy value for xterm-style mouse reporting ASCII sequences */
    TW_XTermMouse          = 0x1FE,

    TW_Null                = 0x1FF,

    TW_KEYCODE_MAX         = (TW_Null+1),
};

typedef enum Twkey_e Twkey;

#endif /* _TW_KEYS_H */

