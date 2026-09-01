default rel
	vpermt2q xmm7, xmm1, xmm5
	vpermt2q xmm0, xmm4, xmm7
	vpermt2q ymm2, ymm3, ymm6
	vpermt2q ymm1, ymm1, yword [0x9e2]
	vpermt2q zmm4, zmm7, zword [0x84d]
	vpermt2q zmm5, zmm7, zmm4
	vpermt2q xmm8, xmm11, xmm8
	vpermt2q ymm12, ymm15, ymm11
	vpermt2q zmm10, zmm12, zmm11
	vpermt2q xmm25, xmm21, xmm17
	vpermt2q ymm21, ymm18, ymm24
	vpermt2q zmm23, zmm22, zmm21
	vpermt2q xmm5{k4}, xmm0, xmm3
	vpermt2q ymm4{k5}, ymm2, ymm7
	vpermt2q zmm1{k5}, zmm2, zmm1
	vpermt2q xmm5{k1}{z}, xmm1, oword [0xb35]
	vpermt2q ymm2{k4}{z}, ymm5, ymm1
	vpermt2q zmm0{k4}{z}, zmm2, zmm3
	vpermt2q xmm3, xmm5, oword [eax+1]
	vpermt2q xmm0, xmm2, oword [eax+64]
	vpermt2q ymm3, ymm0, yword [eax+1]
	vpermt2q ymm7, ymm4, yword [eax+64]
	vpermt2q zmm3, zmm4, zword [eax+1]
	vpermt2q zmm2, zmm6, zword [eax+64]
	vpermt2q xmm3, xmm2, [0x3c8]
	vpermt2q ymm1, ymm3, [0x4a1]
	vpermt2q zmm5, zmm2, [0x6ef]
