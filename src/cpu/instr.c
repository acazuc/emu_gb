#include "instr.h"
#include "cpu.h"

INSTR_DECL(nop);
INSTR_DECL(stop);
INSTR_DECL(halt);
INSTR_DECL(ei);
INSTR_DECL(di);

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
INSTR_DECL(op##_a_n);

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
INSTR_DECL(ld_sp_hl);

INSTR_DECL(ldi_hl_a);
INSTR_DECL(ldi_a_hl);
INSTR_DECL(ldd_hl_a);
INSTR_DECL(ldd_a_hl);
INSTR_DECL(ld_rhl_n);
INSTR_DECL(ld_ffn_a);
INSTR_DECL(ld_ffc_a);
INSTR_DECL(ld_a_ffn);
INSTR_DECL(ld_a_ffc);
INSTR_DECL(ld_a_nn);
INSTR_DECL(ld_nn_a);

INSTR_DECL(add_sp_n);
INSTR_DECL(ld_hl_spn);

INSTR_DECL(inc_rhl);
INSTR_DECL(dec_rhl);

INSTR_DECL(jr_dd);
INSTR_DECL(jr_nz_n);
INSTR_DECL(jr_z_n);
INSTR_DECL(jr_nc_n);
INSTR_DECL(jr_c_n);

INSTR_DECL(jp_nz_nn);
INSTR_DECL(jp_z_nn);
INSTR_DECL(jp_nc_nn);
INSTR_DECL(jp_c_nn);
INSTR_DECL(jp_nn);
INSTR_DECL(jp_hl);

INSTR_DECL(ret_nz);
INSTR_DECL(ret_z);
INSTR_DECL(ret_nc);
INSTR_DECL(ret_c);
INSTR_DECL(reti);
INSTR_DECL(ret);

INSTR_DECL(call_nz_nn);
INSTR_DECL(call_z_nn);
INSTR_DECL(call_nc_nn);
INSTR_DECL(call_c_nn);
INSTR_DECL(call_nn);

INSTR_DECL(rst_00);
INSTR_DECL(rst_08);
INSTR_DECL(rst_10);
INSTR_DECL(rst_18);
INSTR_DECL(rst_20);
INSTR_DECL(rst_28);
INSTR_DECL(rst_30);
INSTR_DECL(rst_38);

INSTR_DECL(push_bc);
INSTR_DECL(pop_bc);
INSTR_DECL(push_de);
INSTR_DECL(pop_de);
INSTR_DECL(push_hl);
INSTR_DECL(pop_hl);
INSTR_DECL(push_af);
INSTR_DECL(pop_af);

#define INSTR_DECL_OP(op) \
INSTR_DECL(op##_a); \
INSTR_DECL(op##_b); \
INSTR_DECL(op##_c); \
INSTR_DECL(op##_d); \
INSTR_DECL(op##_e); \
INSTR_DECL(op##_h); \
INSTR_DECL(op##_l); \
INSTR_DECL(op##_hl);

INSTR_DECL_OP(rlc);
INSTR_DECL_OP(rrc);
INSTR_DECL_OP(rl);
INSTR_DECL_OP(rr);
INSTR_DECL_OP(sla);
INSTR_DECL_OP(sra);
INSTR_DECL_OP(swap);
INSTR_DECL_OP(srl);

#define INSTR_DECL_OP_N(op, n) \
INSTR_DECL(op##_##n##_a); \
INSTR_DECL(op##_##n##_b); \
INSTR_DECL(op##_##n##_c); \
INSTR_DECL(op##_##n##_d); \
INSTR_DECL(op##_##n##_e); \
INSTR_DECL(op##_##n##_h); \
INSTR_DECL(op##_##n##_l); \
INSTR_DECL(op##_##n##_hl);

#define INSTR_DECL_OP_N8(op) \
INSTR_DECL_OP_N(op, 0); \
INSTR_DECL_OP_N(op, 1); \
INSTR_DECL_OP_N(op, 2); \
INSTR_DECL_OP_N(op, 3); \
INSTR_DECL_OP_N(op, 4); \
INSTR_DECL_OP_N(op, 5); \
INSTR_DECL_OP_N(op, 6); \
INSTR_DECL_OP_N(op, 7);

INSTR_DECL_OP_N8(bit);
INSTR_DECL_OP_N8(res);
INSTR_DECL_OP_N8(set);

INSTR_DECL(int_40);
INSTR_DECL(int_48);
INSTR_DECL(int_50);
INSTR_DECL(int_58);
INSTR_DECL(int_60);

INSTR_DEF(unset)
{
	cpu->regs.pc++;
	return true;
}

cpu_instr_t g_cpu_instructions[256] =
{
	/* 0x00 */
	{"nop"        , INSTR_NAME(nop)},
	{"ld bc, nn"  , INSTR_NAME(ld_bc_nn)},
	{"ld (bc), a" , INSTR_NAME(ld_bc_a)},
	{"inc bc"     , INSTR_NAME(inc_bc)},
	{"inc b"      , INSTR_NAME(inc_b)},
	{"dec b"      , INSTR_NAME(dec_b)},
	{"ld b, n"    , INSTR_NAME(ld_b_n)},
	{"rlca"       , INSTR_NAME(rlca)},
	/* 0x08 */
	{"ld (nn), sp", INSTR_NAME(ld_nn_sp)},
	{"add hl, bc" , INSTR_NAME(add_hl_bc)},
	{"ld a, (bc)" , INSTR_NAME(ld_a_bc)},
	{"dec bc"     , INSTR_NAME(dec_bc)},
	{"inc c"      , INSTR_NAME(inc_c)},
	{"dec c"      , INSTR_NAME(dec_c)},
	{"ld c, n"    , INSTR_NAME(ld_c_n)},
	{"rrca"       , INSTR_NAME(rrca)},
	/* 0x10 */
	{"stop"       , INSTR_NAME(stop)},
	{"ld de, nn"  , INSTR_NAME(ld_de_nn)},
	{"ld (de), a" , INSTR_NAME(ld_de_a)},
	{"inc de"     , INSTR_NAME(inc_de)},
	{"inc d"      , INSTR_NAME(inc_d)},
	{"dec d"      , INSTR_NAME(dec_d)},
	{"ld d, n"    , INSTR_NAME(ld_d_n)},
	{"rla"        , INSTR_NAME(rla)},
	/* 0x18 */
	{"jr d"       , INSTR_NAME(jr_dd)},
	{"add hl, de" , INSTR_NAME(add_hl_de)},
	{"ld a, (de)" , INSTR_NAME(ld_a_de)},
	{"dec de"     , INSTR_NAME(dec_de)},
	{"inc e"      , INSTR_NAME(inc_e)},
	{"dec e"      , INSTR_NAME(dec_e)},
	{"ld e, n"    , INSTR_NAME(ld_e_n)},
	{"rra"        , INSTR_NAME(rra)},
	/* 0x20 */
	{"jr nz, d"   , INSTR_NAME(jr_nz_n)},
	{"ld hl, nn"  , INSTR_NAME(ld_hl_nn)},
	{"ldi (hl), a", INSTR_NAME(ldi_hl_a)},
	{"inc hl"     , INSTR_NAME(inc_hl)},
	{"inc h"      , INSTR_NAME(inc_h)},
	{"dec h"      , INSTR_NAME(dec_h)},
	{"ld h, n"    , INSTR_NAME(ld_h_n)},
	{"daa"        , INSTR_NAME(daa)},
	/* 0x28 */
	{"jr z, d"    , INSTR_NAME(jr_z_n)},
	{"add hl, hl" , INSTR_NAME(add_hl_hl)},
	{"ldi a, (hl)", INSTR_NAME(ldi_a_hl)},
	{"dec hl"     , INSTR_NAME(dec_hl)},
	{"inc l"      , INSTR_NAME(inc_l)},
	{"dec l"      , INSTR_NAME(dec_l)},
	{"ld l, n"    , INSTR_NAME(ld_l_n)},
	{"cpl"        , INSTR_NAME(cpl)},
	/* 0x30 */
	{"jr nc, d"   , INSTR_NAME(jr_nc_n)},
	{"ld sp, nn"  , INSTR_NAME(ld_sp_nn)},
	{"ldd (hl), a", INSTR_NAME(ldd_hl_a)},
	{"inc sp"     , INSTR_NAME(inc_sp)},
	{"inc (hl)"   , INSTR_NAME(inc_rhl)},
	{"dec (hl)"   , INSTR_NAME(dec_rhl)},
	{"ld (hl), n" , INSTR_NAME(ld_rhl_n)},
	{"scf"        , INSTR_NAME(scf)},
	/* 0x38 */
	{"jr c, d"    , INSTR_NAME(jr_c_n)},
	{"add hl, sp" , INSTR_NAME(add_hl_sp)},
	{"ldd a, (hl)", INSTR_NAME(ldd_a_hl)},
	{"dec sp"     , INSTR_NAME(dec_sp)},
	{"inc a"      , INSTR_NAME(inc_a)},
	{"dec a"      , INSTR_NAME(dec_a)},
	{"ld a, n"    , INSTR_NAME(ld_a_n)},
	{"ccf"        , INSTR_NAME(ccf)},

#define TBL_LD_R(r) \
	{"ld " #r ", b"   , INSTR_NAME(ld_##r##_b)}, \
	{"ld " #r ", c"   , INSTR_NAME(ld_##r##_c)}, \
	{"ld " #r ", d"   , INSTR_NAME(ld_##r##_d)}, \
	{"ld " #r ", e"   , INSTR_NAME(ld_##r##_e)}, \
	{"ld " #r ", h"   , INSTR_NAME(ld_##r##_h)}, \
	{"ld " #r ", l"   , INSTR_NAME(ld_##r##_l)}, \
	{"ld " #r ", (hl)", INSTR_NAME(ld_##r##_hl)}, \
	{"ld " #r ", a"   , INSTR_NAME(ld_##r##_a)} \

#define TBL_LD_HL_R(r) \
	{"ld (hl), " #r, INSTR_NAME(ld_hl_##r)}

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
	{"ld (hl), b", INSTR_NAME(ld_hl_b)},
	{"ld (hl), c", INSTR_NAME(ld_hl_c)},
	{"ld (hl), d", INSTR_NAME(ld_hl_d)},
	{"ld (hl), e", INSTR_NAME(ld_hl_e)},
	{"ld (hl), h", INSTR_NAME(ld_hl_h)},
	{"ld (hl), l", INSTR_NAME(ld_hl_l)},
	{"halt"      , INSTR_NAME(halt)},
	{"ld (hl), a", INSTR_NAME(ld_hl_a)},
	/* 0x78 */
	TBL_LD_R(a),

#undef TBL_LD_HL_R
#undef TBL_LD_R

#define TBL_OP_A_R(op, r) \
	{#op " a, " #r, INSTR_NAME(op##_a_##r)}

#define TBL_OP_A(op) \
	TBL_OP_A_R(op, b), \
	TBL_OP_A_R(op, c), \
	TBL_OP_A_R(op, d), \
	TBL_OP_A_R(op, e), \
	TBL_OP_A_R(op, h), \
	TBL_OP_A_R(op, l), \
	{#op " a, (hl)", INSTR_NAME(op##_a_hl)}, \
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
	{"ret nz"     , INSTR_NAME(ret_nz)},
	{"pop bc"     , INSTR_NAME(pop_bc)},
	{"jp nz, nn"  , INSTR_NAME(jp_nz_nn)},
	{"jp nn"      , INSTR_NAME(jp_nn)},
	{"call nz, nn", INSTR_NAME(call_nz_nn)},
	{"push bc"    , INSTR_NAME(push_bc)},
	{"add a, n"   , INSTR_NAME(add_a_n)},
	{"rst 00"     , INSTR_NAME(rst_00)},
	/* 0xC8 */
	{"ret z"      , INSTR_NAME(ret_z)},
	{"ret"        , INSTR_NAME(ret)},
	{"jp z, nn"   , INSTR_NAME(jp_z_nn)},
	{"UNSET (cb)" , INSTR_NAME(unset)},
	{"call z, nn" , INSTR_NAME(call_z_nn)},
	{"call nn"    , INSTR_NAME(call_nn)},
	{"adc a, n"   , INSTR_NAME(adc_a_n)},
	{"rst 08"     , INSTR_NAME(rst_08)},
	/* 0xD0 */
	{"ret nc"     , INSTR_NAME(ret_nc)},
	{"pop de"     , INSTR_NAME(pop_de)},
	{"jp nc, nn"  , INSTR_NAME(jp_nc_nn)},
	{"UNSET (d3)" , INSTR_NAME(unset)},
	{"call nc, nn", INSTR_NAME(call_nc_nn)},
	{"push de"    , INSTR_NAME(push_de)},
	{"sub a, n"   , INSTR_NAME(sub_a_n)},
	{"rst 10"     , INSTR_NAME(rst_10)},
	/* 0xD8 */
	{"ret c"      , INSTR_NAME(ret_c)},
	{"reti"       , INSTR_NAME(reti)},
	{"jp c, nn"   , INSTR_NAME(jp_c_nn)},
	{"UNSET (db)" , INSTR_NAME(unset)},
	{"call c, nn" , INSTR_NAME(call_c_nn)},
	{"UNSET (dd)" , INSTR_NAME(unset)},
	{"sbc a, n"   , INSTR_NAME(sbc_a_n)},
	{"rst 18"     , INSTR_NAME(rst_18)},
	/* 0xE0 */
	{"ld (ff+n), a", INSTR_NAME(ld_ffn_a)},
	{"pop hl"      , INSTR_NAME(pop_hl)},
	{"ld (ff+c), a", INSTR_NAME(ld_ffc_a)},
	{"UNSET (e3)"  , INSTR_NAME(unset)},
	{"UNSET (e4)"  , INSTR_NAME(unset)},
	{"push hl"     , INSTR_NAME(push_hl)},
	{"and a, n"    , INSTR_NAME(and_a_n)},
	{"rst 20"      , INSTR_NAME(rst_20)},
	/* 0xE8 */
	{"add sp, n"   , INSTR_NAME(add_sp_n)},
	{"jp hl"       , INSTR_NAME(jp_hl)},
	{"ld (nn), a"  , INSTR_NAME(ld_nn_a)},
	{"UNSET (eb)"  , INSTR_NAME(unset)},
	{"UNSET (ec)"  , INSTR_NAME(unset)},
	{"UNSET (ed)"  , INSTR_NAME(unset)},
	{"xor a, n"    , INSTR_NAME(xor_a_n)},
	{"rst 28"      , INSTR_NAME(rst_28)},
	/* 0xF0 */
	{"ld a, (ff+n)", INSTR_NAME(ld_a_ffn)},
	{"pop af"      , INSTR_NAME(pop_af)},
	{"ld a, (ff+c)", INSTR_NAME(ld_a_ffc)},
	{"di"          , INSTR_NAME(di)},
	{"UNSET (f4)"  , INSTR_NAME(unset)},
	{"push af"     , INSTR_NAME(push_af)},
	{"or a, n"     , INSTR_NAME(or_a_n)},
	{"rst 30"      , INSTR_NAME(rst_30)},
	/* 0xF8 */
	{"ld hl, sp+n" , INSTR_NAME(ld_hl_spn)},
	{"ld sp, hl"   , INSTR_NAME(ld_sp_hl)},
	{"ld a, nn"    , INSTR_NAME(ld_a_nn)},
	{"ei"          , INSTR_NAME(ei)},
	{"UNSET (fc)"  , INSTR_NAME(unset)},
	{"UNSET (fd)"  , INSTR_NAME(unset)},
	{"cp a, n"     , INSTR_NAME(cp_a_n)},
	{"rst 38"      , INSTR_NAME(rst_38)}
};

cpu_instr_t g_cpu_cb_instructions[256] =
{
#define TBL_OP(op) \
	{#op " b"   , INSTR_NAME(op##_b)}, \
	{#op " c"   , INSTR_NAME(op##_c)}, \
	{#op " d"   , INSTR_NAME(op##_d)}, \
	{#op " e"   , INSTR_NAME(op##_e)}, \
	{#op " h"   , INSTR_NAME(op##_h)}, \
	{#op " l"   , INSTR_NAME(op##_l)}, \
	{#op " (hl)", INSTR_NAME(op##_hl)}, \
	{#op " a"   , INSTR_NAME(op##_a)}

	/* 0x00 */
	TBL_OP(rlc),
	/* 0x08 */
	TBL_OP(rrc),
	/* 0x10 */
	TBL_OP(rl),
	/* 0x18 */
	TBL_OP(rr),
	/* 0x20 */
	TBL_OP(sla),
	/* 0x28 */
	TBL_OP(sra),
	/* 0x30 */
	TBL_OP(swap),
	/* 0x38 */
	TBL_OP(srl),

#undef TBL_OP

#define TBL_OP_N(op, n) \
	{#op " " #n ", b"   , INSTR_NAME(op##_##n##_b)}, \
	{#op " " #n ", c"   , INSTR_NAME(op##_##n##_c)}, \
	{#op " " #n ", d"   , INSTR_NAME(op##_##n##_d)}, \
	{#op " " #n ", e"   , INSTR_NAME(op##_##n##_e)}, \
	{#op " " #n ", h"   , INSTR_NAME(op##_##n##_h)}, \
	{#op " " #n ", l"   , INSTR_NAME(op##_##n##_l)}, \
	{#op " " #n ", (hl)", INSTR_NAME(op##_##n##_hl)}, \
	{#op " " #n ", a"   , INSTR_NAME(op##_##n##_a)} \

	/* 0x40 */
	TBL_OP_N(bit, 0),
	/* 0x48 */
	TBL_OP_N(bit, 1),
	/* 0x50 */
	TBL_OP_N(bit, 2),
	/* 0x58 */
	TBL_OP_N(bit, 3),
	/* 0x60 */
	TBL_OP_N(bit, 4),
	/* 0x68 */
	TBL_OP_N(bit, 5),
	/* 0x70 */
	TBL_OP_N(bit, 6),
	/* 0x78 */
	TBL_OP_N(bit, 7),
	/* 0x80 */
	TBL_OP_N(res, 0),
	/* 0x88 */
	TBL_OP_N(res, 1),
	/* 0x90 */
	TBL_OP_N(res, 2),
	/* 0x98 */
	TBL_OP_N(res, 3),
	/* 0xA0 */
	TBL_OP_N(res, 4),
	/* 0xA8 */
	TBL_OP_N(res, 5),
	/* 0xB0 */
	TBL_OP_N(res, 6),
	/* 0xB8 */
	TBL_OP_N(res, 7),
	/* 0xC0 */
	TBL_OP_N(set, 0),
	/* 0xC8 */
	TBL_OP_N(set, 1),
	/* 0xD0 */
	TBL_OP_N(set, 2),
	/* 0xD8 */
	TBL_OP_N(set, 3),
	/* 0xE0 */
	TBL_OP_N(set, 4),
	/* 0xE8 */
	TBL_OP_N(set, 5),
	/* 0xF0 */
	TBL_OP_N(set, 6),
	/* 0xF8 */
	TBL_OP_N(set, 7),

#undef TBL_OP_N
};

cpu_instr_t g_cpu_int_instructions[5] =
{
	{"int 40", INSTR_NAME(int_40)},
	{"int 48", INSTR_NAME(int_48)},
	{"int 50", INSTR_NAME(int_50)},
	{"int 58", INSTR_NAME(int_58)},
	{"int 60", INSTR_NAME(int_60)},
};
