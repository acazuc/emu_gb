#include "instr.h"

INSTR_DECL(nop);
INSTR_DECL(stop);
INSTR_DECL(halt);

#define INSTR_DECL_R(r) \
INSTR_DECL(inc_##r); \
INSTR_DECL(dec_##r); \
INSTR_DECL(ld_##r##_n);

#define INSTR_DECL_RR(rr) \
INSTR_DECL(inc_##rr); \
INSTR_DECL(dec_##rr); \
INSTR_DECL(ld_##rr##_a); \
INSTR_DECL(ld_a_##rr); \
INSTR_DECL(ld_##rr##_nn); \
INSTR_DECL(add_hl_##rr);

#define INSTR_DECL_LD_R_R(r1, r2) \
INSTR_DECL(ld_##r1##_##r2);

#define INSTR_DECL_LD_R(r) \
INSTR_DECL_LD_R_R(r, a); \
INSTR_DECL_LD_R_R(r, b); \
INSTR_DECL_LD_R_R(r, c); \
INSTR_DECL_LD_R_R(r, d); \
INSTR_DECL_LD_R_R(r, e); \
INSTR_DECL_LD_R_R(r, h); \
INSTR_DECL_LD_R_R(r, l); \
INSTR_DECL(ld_##r##_hl); \
INSTR_DECL(ld_hl_##r);

INSTR_DECL_LD_R(a);
INSTR_DECL_LD_R(b);
INSTR_DECL_LD_R(c);
INSTR_DECL_LD_R(d);
INSTR_DECL_LD_R(e);
INSTR_DECL_LD_R(h);
INSTR_DECL_LD_R(l);

#define INSTR_DECL_OP_A_R(op, r) \
INSTR_DECL(op##_a_##r);

#define INSTR_DECL_OP_A(op) \
INSTR_DECL_OP_A_R(op, a); \
INSTR_DECL_OP_A_R(op, b); \
INSTR_DECL_OP_A_R(op, c); \
INSTR_DECL_OP_A_R(op, d); \
INSTR_DECL_OP_A_R(op, e); \
INSTR_DECL_OP_A_R(op, h); \
INSTR_DECL_OP_A_R(op, l); \
INSTR_DECL_OP_A_R(op, hl); \

INSTR_DECL_OP_A(add);
INSTR_DECL_OP_A(adc);
INSTR_DECL_OP_A(sub);
INSTR_DECL_OP_A(sbc);
INSTR_DECL_OP_A(and);
INSTR_DECL_OP_A(xor);
INSTR_DECL_OP_A(or);
INSTR_DECL_OP_A(cp);

INSTR_DECL_R(a);
INSTR_DECL_R(b);
INSTR_DECL_R(c);
INSTR_DECL_R(d);
INSTR_DECL_R(e);
INSTR_DECL_R(h);
INSTR_DECL_R(l);

INSTR_DECL_RR(bc);
INSTR_DECL_RR(de);
INSTR_DECL_RR(hl);
INSTR_DECL_RR(sp);

INSTR_DECL(rlca);
INSTR_DECL(rrca);
INSTR_DECL(rla);
INSTR_DECL(rra);

INSTR_DECL(daa);
INSTR_DECL(cpl);
INSTR_DECL(scf);
INSTR_DECL(ccf);

INSTR_DECL(ld_nn_sp);

INSTR_DECL(ldi_hl_a);
INSTR_DECL(ldi_a_hl);
INSTR_DECL(ldd_hl_a);
INSTR_DECL(ldd_a_hl);
INSTR_DECL(ld_rhl_n);

INSTR_DECL(inc_rhl);
INSTR_DECL(dec_rhl);

INSTR_DECL(jr_dd);
INSTR_DECL(jr_nz_n);
INSTR_DECL(jr_z_n);
INSTR_DECL(jr_nc_n);
INSTR_DECL(jr_c_n);

z80_instr_t g_z80_instructions[256] =
{
	/* 0x00 */
	{"nop"        , 1, 1, INSTR_NAME(nop)},
	{"ld bc, nn"  , 3, 3, INSTR_NAME(ld_bc_nn)},
	{"ld (bc), a" , 1, 2, INSTR_NAME(ld_bc_a)},
	{"inc bc"     , 1, 2, INSTR_NAME(inc_bc)},
	{"inc b"      , 1, 1, INSTR_NAME(inc_b)},
	{"dec b"      , 1, 1, INSTR_NAME(dec_b)},
	{"ld b, n"    , 2, 2, INSTR_NAME(ld_b_n)},
	{"rlca"       , 1, 1, INSTR_NAME(rlca)},
	/* 0x08 */
	{"ld (nn), sp", 3, 5, INSTR_NAME(ld_nn_sp)},
	{"add hl, bc" , 1, 2, INSTR_NAME(add_hl_bc)},
	{"ld a, (bc)" , 1, 2, INSTR_NAME(ld_a_bc)},
	{"dec bc"     , 1, 2, INSTR_NAME(dec_bc)},
	{"inc c"      , 1, 1, INSTR_NAME(inc_c)},
	{"dec c"      , 1, 1, INSTR_NAME(dec_c)},
	{"ld c, n"    , 2, 2, INSTR_NAME(ld_c_n)},
	{"rrca"       , 1, 1, INSTR_NAME(rrca)},
	/* 0x10 */
	{"stop"       , 1, 1, INSTR_NAME(stop)},
	{"ld de, nn"  , 3, 3, INSTR_NAME(ld_de_nn)},
	{"ld (de), a" , 1, 2, INSTR_NAME(ld_de_a)},
	{"inc de"     , 1, 2, INSTR_NAME(inc_de)},
	{"inc d"      , 1, 1, INSTR_NAME(inc_d)},
	{"dec d"      , 1, 1, INSTR_NAME(dec_d)},
	{"ld d, n"    , 2, 2, INSTR_NAME(ld_d_n)},
	{"rla"        , 1, 1, INSTR_NAME(rla)},
	/* 0x18 */
	{"jr dd"      , 2, 3, INSTR_NAME(jr_dd)},
	{"add hl, de" , 1, 2, INSTR_NAME(add_hl_de)},
	{"ld a, (de)" , 1, 2, INSTR_NAME(ld_a_de)},
	{"dec de"     , 1, 2, INSTR_NAME(dec_de)},
	{"inc e"      , 1, 1, INSTR_NAME(inc_e)},
	{"dec e"      , 1, 1, INSTR_NAME(dec_e)},
	{"ld e, n"    , 2, 2, INSTR_NAME(ld_e_n)},
	{"rra"        , 1, 1, INSTR_NAME(rra)},
	/* 0x20 */
	{"jr nz, n"   , 2, 2, INSTR_NAME(jr_nz_n)},
	{"ld hl, nn"  , 3, 3, INSTR_NAME(ld_hl_nn)},
	{"ldi (hl), a", 1, 2, INSTR_NAME(ldi_hl_a)},
	{"inc hl"     , 1, 2, INSTR_NAME(inc_hl)},
	{"inc h"      , 1, 1, INSTR_NAME(inc_h)},
	{"dec h"      , 1, 1, INSTR_NAME(dec_h)},
	{"ld h, n"    , 2, 2, INSTR_NAME(ld_h_n)},
	{"daa"        , 1, 1, INSTR_NAME(daa)},
	/* 0x28 */
	{"jr z, n"    , 2, 2, INSTR_NAME(jr_z_n)},
	{"add hl, hl" , 1, 2, INSTR_NAME(add_hl_hl)},
	{"ldi a, (hl)", 1, 2, INSTR_NAME(ldi_a_hl)},
	{"dec hl"     , 1, 2, INSTR_NAME(dec_hl)},
	{"inc l"      , 1, 1, INSTR_NAME(inc_l)},
	{"dec l"      , 1, 1, INSTR_NAME(dec_l)},
	{"ld l, n"    , 2, 2, INSTR_NAME(ld_l_n)},
	{"cpl"        , 1, 1, INSTR_NAME(cpl)},
	/* 0x30 */
	{"jr nc, n"   , 2, 2, INSTR_NAME(jr_nc_n)},
	{"ld sp, nn"  , 3, 3, INSTR_NAME(ld_sp_nn)},
	{"ldd (hl), a", 1, 2, INSTR_NAME(ldd_hl_a)},
	{"inc sp"     , 1, 2, INSTR_NAME(inc_sp)},
	{"inc (hl)"   , 1, 3, INSTR_NAME(inc_rhl)},
	{"dec (hl)"   , 1, 3, INSTR_NAME(dec_rhl)},
	{"ld (hl), n" , 2, 3, INSTR_NAME(ld_rhl_n)},
	{"scf"        , 1, 1, INSTR_NAME(scf)},
	/* 0x38 */
	{"jr c, n"    , 2, 2, INSTR_NAME(jr_c_n)},
	{"add hl, sp" , 1, 2, INSTR_NAME(add_hl_sp)},
	{"ldd a, (hl)", 1, 2, INSTR_NAME(ldd_a_hl)},
	{"dec sp"     , 1, 2, INSTR_NAME(dec_sp)},
	{"inc a"      , 1, 1, INSTR_NAME(inc_a)},
	{"dec a"      , 1, 1, INSTR_NAME(dec_a)},
	{"ld a, n"    , 2, 2, INSTR_NAME(ld_a_n)},
	{"ccf"        , 1, 1, INSTR_NAME(ccf)},

#define TBL_LD_R_R(r1, r2) \
	{"ld " #r1 ", " #r2, 1, 1, INSTR_NAME(ld_##r1##_##r2)}

#define TBL_LD_R(r) \
	TBL_LD_R_R(r, b), \
	TBL_LD_R_R(r, c), \
	TBL_LD_R_R(r, d), \
	TBL_LD_R_R(r, e), \
	TBL_LD_R_R(r, h), \
	TBL_LD_R_R(r, l), \
	{"ld " #r ", (hl)", 1, 2, INSTR_NAME(ld_##r##_hl)}, \
	TBL_LD_R_R(r, a)

#define TBL_LD_HL_R(r) \
	{"ld (hl), " #r, 1, 1, INSTR_NAME(ld_hl_##r)}

	/* 0x40 */
	TBL_LD_R(b),
	/* 0x48 */
	TBL_LD_R(c),
	/* 0x50 */
	TBL_LD_R(d),
	/* 0x58 */
	TBL_LD_R(e),
	/* 0x60 */
	TBL_LD_R(h),
	/* 0x68 */
	TBL_LD_R(l),
	/* 0x70 */
	TBL_LD_HL_R(b),
	TBL_LD_HL_R(c),
	TBL_LD_HL_R(d),
	TBL_LD_HL_R(e),
	TBL_LD_HL_R(h),
	TBL_LD_HL_R(l),
	{"halt", 1, 1, INSTR_NAME(halt)},
	TBL_LD_HL_R(a),
	/* 0x78 */
	TBL_LD_R(a),

#undef TBL_LD_HL_R
#undef TBL_LD_R
#undef TBL_LD_R_R

#define TBL_OP_A_R(op, r) \
	{#op " a, " #r, 1, 1, INSTR_NAME(op##_a_##r)}

#define TBL_OP_A(op) \
	TBL_OP_A_R(op, b), \
	TBL_OP_A_R(op, c), \
	TBL_OP_A_R(op, d), \
	TBL_OP_A_R(op, e), \
	TBL_OP_A_R(op, h), \
	TBL_OP_A_R(op, l), \
	{#op " a, (hl)", 1, 2, INSTR_NAME(op##_a_hl)}, \
	TBL_OP_A_R(op, a) \

	/* 0x80 */
	TBL_OP_A(add),
	/* 0x88 */
	TBL_OP_A(adc),
	/* 0x90 */
	TBL_OP_A(sub),
	/* 0x98 */
	TBL_OP_A(sbc),
	/* 0xA0 */
	TBL_OP_A(and),
	/* 0xA8 */
	TBL_OP_A(xor),
	/* 0xB0 */
	TBL_OP_A(or),
	/* 0xB8 */
	TBL_OP_A(cp),

#undef TBL_OP_A
#undef TBL_OP_A_R

	/* 0xC0 */
};
