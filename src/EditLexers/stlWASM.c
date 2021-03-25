#include "EditLexer.h"
#include "EditStyleX.h"

// https://webassembly.github.io/spec/core/text/index.html
// https://github.com/WebAssembly/wabt

static KEYWORDLIST Keywords_WASM = {{
//++Autogenerated -- start of section automatically generated
"array assert_exhaustion assert_invalid assert_malformed assert_return assert_trap assert_unlinkable binary "
"data declare do elem event exn export extern field func get global import input invoke item local memory module mut "
"nan:arithmetic nan:canonical offset output param quote ref.extern register result shared start struct table then type "

, // 1 type
"anyfunc exnref externref f32 f32x4 f64 f64x2 funcref i16x8 i32 i32x4 i64 i64x2 i8x16 v128 v16x8 v32x4 v64x2 v8x16 "

, // 2 instruction
"atomic.fence atomic.notify block br br_if br_on_exn br_table call call_indirect catch current_memory data.drop drop "
"elem.drop else end get_global get_local global.get global.set grow_memory if local.get local.set local.tee loop "
"memory.copy memory.fill memory.grow memory.init memory.size nop "
"ref.func ref.is_null ref.null rethrow return return_call return_call_indirect select set_global set_local "
"table.copy table.fill table.get table.grow table.init table.set table.size tee_local throw try unreachable "

, // 3 full instruction
"f32.abs f32.add f32.ceil f32.const f32.convert_i32_s f32.convert_i32_u f32.convert_i64_s f32.convert_i64_u f32.copysign "
"f32.demote_f64 f32.div f32.eq f32.floor f32.ge f32.gt f32.le f32.load f32.lt f32.max f32.min f32.mul "
"f32.ne f32.nearest f32.neg f32.reinterpret_i32 f32.sqrt f32.store f32.sub f32.trunc f32x4.abs f32x4.add "
"f32x4.convert_i32x4_s f32x4.convert_i32x4_u f32x4.div f32x4.eq f32x4.extract_lane f32x4.ge f32x4.gt f32x4.le f32x4.lt "
"f32x4.max f32x4.min f32x4.mul f32x4.ne f32x4.neg f32x4.replace_lane f32x4.splat f32x4.sqrt f32x4.sub f64.abs f64.add "
"f64.ceil f64.const f64.convert_i32_s f64.convert_i32_u f64.convert_i64_s f64.convert_i64_u f64.copysign f64.div f64.eq "
"f64.floor f64.ge f64.gt f64.le f64.load f64.lt f64.max f64.min f64.mul f64.ne f64.nearest f64.neg f64.promote_f32 "
"f64.reinterpret_i64 f64.sqrt f64.store f64.sub f64.trunc f64x2.abs f64x2.add f64x2.div f64x2.eq f64x2.extract_lane "
"f64x2.ge f64x2.gt f64x2.le f64x2.lt f64x2.max f64x2.min f64x2.mul f64x2.ne f64x2.neg f64x2.replace_lane "
"f64x2.splat f64x2.sqrt f64x2.sub "
"i16x8.abs i16x8.add i16x8.add_saturate_s i16x8.add_saturate_u i16x8.all_true i16x8.any_true i16x8.avgr_u "
"i16x8.eq i16x8.extract_lane_s i16x8.extract_lane_u i16x8.ge_s i16x8.ge_u i16x8.gt_s i16x8.gt_u "
"i16x8.le_s i16x8.le_u i16x8.load8x8_s i16x8.load8x8_u i16x8.lt_s i16x8.lt_u "
"i16x8.max_s i16x8.max_u i16x8.min_s i16x8.min_u i16x8.mul i16x8.narrow_i32x4_s i16x8.narrow_i32x4_u i16x8.ne i16x8.neg "
"i16x8.replace_lane i16x8.shl i16x8.shr_s i16x8.shr_u i16x8.splat i16x8.sub i16x8.sub_saturate_s i16x8.sub_saturate_u "
"i16x8.widen_high_i8x16_s i16x8.widen_high_i8x16_u i16x8.widen_low_i8x16_s i16x8.widen_low_i8x16_u i32.add i32.and "
"i32.atomic.load i32.atomic.load16_u i32.atomic.load8_u i32.atomic.rmw.add i32.atomic.rmw.and i32.atomic.rmw.cmpxchg "
"i32.atomic.rmw.or i32.atomic.rmw.sub i32.atomic.rmw.xchg i32.atomic.rmw.xor "
"i32.atomic.rmw16.add_u i32.atomic.rmw16.and_u i32.atomic.rmw16.cmpxchg_u i32.atomic.rmw16.or_u i32.atomic.rmw16.sub_u "
"i32.atomic.rmw16.xchg_u i32.atomic.rmw16.xor_u i32.atomic.rmw8.add_u i32.atomic.rmw8.and_u i32.atomic.rmw8.cmpxchg_u "
"i32.atomic.rmw8.or_u i32.atomic.rmw8.sub_u i32.atomic.rmw8.xchg_u i32.atomic.rmw8.xor_u "
"i32.atomic.store i32.atomic.store16 i32.atomic.store8 i32.atomic.wait i32.clz i32.const i32.ctz i32.div_s i32.div_u "
"i32.eq i32.eqz i32.extend16_s i32.extend8_s i32.ge_s i32.ge_u i32.gt_s i32.gt_u "
"i32.le_s i32.le_u i32.load i32.load16_s i32.load16_u i32.load8_s i32.load8_u i32.lt_s i32.lt_u i32.mul i32.ne i32.or "
"i32.popcnt i32.reinterpret_f32 i32.rem_s i32.rem_u i32.rotl i32.rotr "
"i32.shl i32.shr_s i32.shr_u i32.store i32.store16 i32.store8 i32.sub "
"i32.trunc_f32_s i32.trunc_f32_u i32.trunc_f64_s i32.trunc_f64_u "
"i32.trunc_sat_f32_s i32.trunc_sat_f32_u i32.trunc_sat_f64_s i32.trunc_sat_f64_u i32.wrap_i64 i32.xor "
"i32x4.abs i32x4.add i32x4.all_true i32x4.any_true i32x4.eq i32x4.extract_lane "
"i32x4.ge_s i32x4.ge_u i32x4.gt_s i32x4.gt_u "
"i32x4.le_s i32x4.le_u i32x4.load16x4_s i32x4.load16x4_u i32x4.lt_s i32x4.lt_u "
"i32x4.max_s i32x4.max_u i32x4.min_s i32x4.min_u i32x4.mul i32x4.ne i32x4.neg i32x4.replace_lane "
"i32x4.shl i32x4.shr_s i32x4.shr_u i32x4.splat i32x4.sub i32x4.trunc_sat_f32x4_s i32x4.trunc_sat_f32x4_u "
"i32x4.widen_high_i16x8_s i32x4.widen_high_i16x8_u i32x4.widen_low_i16x8_s i32x4.widen_low_i16x8_u i64.add i64.and "
"i64.atomic.load i64.atomic.load16_u i64.atomic.load32_u i64.atomic.load8_u i64.atomic.rmw.add i64.atomic.rmw.and "
"i64.atomic.rmw.cmpxchg i64.atomic.rmw.or i64.atomic.rmw.sub i64.atomic.rmw.xchg i64.atomic.rmw.xor "
"i64.atomic.rmw16.add_u i64.atomic.rmw16.and_u i64.atomic.rmw16.cmpxchg_u i64.atomic.rmw16.or_u i64.atomic.rmw16.sub_u "
"i64.atomic.rmw16.xchg_u i64.atomic.rmw16.xor_u i64.atomic.rmw32.add_u i64.atomic.rmw32.and_u i64.atomic.rmw32.cmpxchg_u "
"i64.atomic.rmw32.or_u i64.atomic.rmw32.sub_u i64.atomic.rmw32.xchg_u i64.atomic.rmw32.xor_u "
"i64.atomic.rmw8.add_u i64.atomic.rmw8.and_u i64.atomic.rmw8.cmpxchg_u i64.atomic.rmw8.or_u i64.atomic.rmw8.sub_u "
"i64.atomic.rmw8.xchg_u i64.atomic.rmw8.xor_u i64.atomic.store i64.atomic.store16 i64.atomic.store32 i64.atomic.store8 "
"i64.atomic.wait i64.clz i64.const i64.ctz i64.div_s i64.div_u "
"i64.eq i64.eqz i64.extend16_s i64.extend32_s i64.extend8_s i64.extend_i32_s i64.extend_i32_u "
"i64.ge_s i64.ge_u i64.gt_s i64.gt_u i64.le_s i64.le_u "
"i64.load i64.load16_s i64.load16_u i64.load32_s i64.load32_u i64.load8_s i64.load8_u i64.lt_s i64.lt_u i64.mul i64.ne "
"i64.or i64.popcnt i64.reinterpret_f64 i64.rem_s i64.rem_u i64.rotl i64.rotr "
"i64.shl i64.shr_s i64.shr_u i64.store i64.store16 i64.store32 i64.store8 i64.sub "
"i64.trunc_f32_s i64.trunc_f32_u i64.trunc_f64_s i64.trunc_f64_u "
"i64.trunc_sat_f32_s i64.trunc_sat_f32_u i64.trunc_sat_f64_s i64.trunc_sat_f64_u i64.xor i64x2.add i64x2.extract_lane "
"i64x2.load32x2_s i64x2.load32x2_u i64x2.mul i64x2.neg i64x2.replace_lane "
"i64x2.shl i64x2.shr_s i64x2.shr_u i64x2.splat i64x2.sub "
"i8x16.abs i8x16.add i8x16.add_saturate_s i8x16.add_saturate_u i8x16.all_true i8x16.any_true i8x16.avgr_u "
"i8x16.eq i8x16.extract_lane_s i8x16.extract_lane_u i8x16.ge_s i8x16.ge_u i8x16.gt_s i8x16.gt_u "
"i8x16.le_s i8x16.le_u i8x16.lt_s i8x16.lt_u i8x16.max_s i8x16.max_u i8x16.min_s i8x16.min_u "
"i8x16.narrow_i16x8_s i8x16.narrow_i16x8_u i8x16.ne i8x16.neg i8x16.replace_lane "
"i8x16.shl i8x16.shr_s i8x16.shr_u i8x16.splat i8x16.sub i8x16.sub_saturate_s i8x16.sub_saturate_u "
"v128.and v128.andnot v128.bitselect v128.const v128.load v128.not v128.or v128.store v128.xor v16x8.load_splat "
"v32x4.load_splat v64x2.load_splat v8x16.load_splat v8x16.shuffle v8x16.swizzle "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_WASM[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_WASM_KEYWORD, NP2StyleX_Keyword, L"fore:#FF8000" },
	{ SCE_WASM_TYPE, NP2StyleX_Type, L"fore:#7C5AF3" },
	{ SCE_WASM_INSTRUCTION, NP2StyleX_Instruction, L"fore:#0080FF" },
	{ MULTI_STYLE(SCE_WASM_COMMENTLINE, SCE_WASM_COMMENTBLOCK, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ SCE_WASM_STRING, NP2StyleX_String, L"fore:#008000" },
	{ SCE_WASM_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_WASM_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_WASM_OPERATOR, NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexWASM = {
	SCLEX_WASM, NP2LEX_WASM,
	SCHEME_SETTINGS_DEFAULT,
	EDITLEXER_HOLE(L"WebAssembly", Styles_WASM),
	L"wat; wast",
	&Keywords_WASM,
	Styles_WASM
};
