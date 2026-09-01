default rel
	vpermt2d xmm7, xmm2, xmm6
	vpermt2d xmm4, xmm4, xmm7
	vpermt2d ymm6, ymm7, ymm5
	vpermt2d ymm4, ymm6, ymm7
	vpermt2d zmm2, zmm0, zword [0x10e]
	vpermt2d zmm5, zmm3, zmm7
	vpermt2d xmm8, xmm10, xmm12
	vpermt2d ymm8, ymm13, ymm8
	vpermt2d zmm14, zmm13, zmm12
	vpermt2d xmm28, xmm25, xmm24
	vpermt2d ymm17, ymm23, ymm16
	vpermt2d zmm18, zmm31, zmm28
	vpermt2d xmm5{k6}, xmm0, xmm2
	vpermt2d ymm3{k7}, ymm4, ymm3
	vpermt2d zmm6{k5}, zmm1, zword [0x7ae]
	vpermt2d xmm5{k7}{z}, xmm0, oword [0x669]
	vpermt2d ymm5{k5}{z}, ymm6, ymm2
	vpermt2d zmm6{k7}{z}, zmm3, zmm5
	vpermt2d xmm0, xmm5, oword [eax+1]
	vpermt2d xmm1, xmm5, oword [eax+64]
	vpermt2d ymm5, ymm4, yword [eax+1]
	vpermt2d ymm4, ymm1, yword [eax+64]
	vpermt2d zmm5, zmm2, zword [eax+1]
	vpermt2d zmm6, zmm6, zword [eax+64]
	vpermt2d xmm3, xmm3, [0x936]
	vpermt2d ymm1, ymm5, [0xc70]
	vpermt2d zmm0, zmm6, [0xb59]
