;
; This file was automatically generated from m4/ltrace.conf.m4, do not edit!
;

define(`DEF', `define(DEF_$1)$@')

include(`m4/ltrace.conf.libTw.in')

divert(-1)

undefine(`DEF')

define(`TRIM', `translit(`$1', ` ')')

define(`CHAIN', `TRIM($1)`'TRIM($2)')

define(`NAME', `Tw_`'CHAIN($1,$2)')

define(`NSHIFT', `ifelse($1, 0, `shift($@)', `NSHIFT(decr($1), shift(shift($@)))')')

define(`tv', v)
define(`t_', _)
define(`tx', x)
define(`tV', V)
define(`tW', W)
define(`tX', X)
define(`tY', Y)

define(`iV', `$1')
define(`iW', `$1')
define(`iX', `$1')
define(`iY', `$1')


define(`T_byte',  `uint')
define(`T_sbyte', `int')
define(`T_dat',   `int')
define(`T_udat',  `uint')
define(`T_ldat',  `int')
define(`T_uldat', `uint')
define(`T_time_t', `int')
define(`T_frac_t', `int')
define(`T_hwfont', `addr')
define(`T_hwcol', `addr')
define(`T_hwattr', `addr')
define(`T_opaque', `addr')
define(`T_any', `uint')

define(`Tv', `void')
define(`T_', `T_$1')
define(`Tx', `addr')
define(`TV', `ifelse(`$1', char, string, `$1', byte, string, addr)')
define(`TW', `TV($@)')
define(`TX', `addr')
define(`TY', `addr')

define(`TYPE', `T$2($1)')

define(`A')

define(`NARGS', `eval(($1-5)/2)')
define(`NARGS1', `eval(1+($1-5)/2)')

define(`ARG', `ifelse($3, v, `', `TYPE($2,$3)`'A($1)')')

define(`_ARGS', `ifelse($#, 2, `', `, ARG($1,$2,t$3)`'_ARGS(incr($1), NSHIFT(3, $@))')')
define(`ARGS', `addr`'ifelse($#, 2, `', `_ARGS($@)')')

define(`PROTO', `ifdef(DEF_`'NAME($3, $4), `', `TYPE($1,t$2) NAME($3, $4)(ARGS(1, NSHIFT(5, $@)));define(DEF_`'NAME($3, $4))')')

define(`PROTOSyncSocket', `PROTO($@)')

define(`PROTOFindFunction', `PROTO($@)')

divert

include(`m4/sockproto.m4h')

dnl
dnl
dnl now libTT
dnl
dnl

define(`DEF', `define(DEF_$1)$@')

include(`m4/ltrace.conf.libTT.in')

undefine(`DEF')

include(`m4/TTclasses.m4h')
include(`m4/TThandy.m4h')

divert(-1)


define(`arg_void',    `void')
define(`arg_ttbyte',  `uint')
define(`arg_ttsbyte', `int')
define(`arg_ttshort', `int')
define(`arg_ttushort',`uint')
define(`arg_ttint',   `int')
define(`arg_ttuint',  `uint')

define(`arg_decay_scalar', `ifelse(index(`$1', `_fn'), -1, `ifdef(`arg_$1', arg_$1, addr)', addr)')
define(`arg_decay_pointer', `ifelse(index(`$1', `ttbyte'), -1, addr, string)')

define(`arg_decay', `ifdef(`m4super_$1', addr, `ifelse(index(`$1', `*'), -1, `arg_decay_scalar(`$1')', `arg_decay_pointer(`$1')')')')

define(`_args_decay', `ifelse(`$1', `', `', `, arg_decay(`$1')`'_args_decay(shift($@))')')
define(`args_decay', `ifelse(`$2', `', `arg_decay(`$1')', `arg_decay(`$1')`'_args_decay(shift($@))')')

define(`TTdeclare',`ifdef(`DEF_TT$2_$3', `', `
arg_decay($5) TT$2_$3(ifelse($4, 0, `void', `args_decay(NSHIFT(5, $@))'));define(`DEF_TT$2_$3')')')

define(`el',`TTFNdef_$1($1)')

divert

define(`extends')
define(`public',`TTdeclare($@)')
define(`exported',`public($@)')
TTlist()
define(`exported')
define(`public')
define(`extends')
undefine(`el')