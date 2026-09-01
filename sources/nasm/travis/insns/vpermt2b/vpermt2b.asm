default rel
	vpermt2b xmm5, xmm2, xmm5
	vpermt2b xmm1, xmm3, xmm6
	vpermt2b ymm1, ymm6, ymm1
	vpermt2b ymm4, ymm3, ymm4
	vpermt2b zmm2, zmm7, zmm7
	vpermt2b zmm0, zmm7, zmm0
	vpermt2b xmm10, xmm13, xmm8
	vpermt2b ymm10, ymm8, ymm12
	vpermt2b zmm12, zmm9, zmm14
	vpermt2b xmm27, xmm18, xmm17
	vpermt2b ymm18, ymm28, ymm20
	vpermt2b zmm22, zmm26, zmm20
	vpermt2b xmm1{k1}, xmm1, xmm5
	vpermt2b ymm1{k4}, ymm6, ymm3
	vpermt2b zmm0{k3}, zmm0, zmm6
	vpermt2b xmm0{k1}{z}, xmm0, xmm3
	vpermt2b ymm2{k4}{z}, ymm2, yword [0x2ca]
	vpermt2b zmm6{k4}{z}, zmm3, zword [0x1a0]
	vpermt2b xmm6, xmm6, oword [eax+1]
	vpermt2b xmm5, xmm7, oword [eax+64]
	vpermt2b ymm4, ymm4, yword [eax+1]
	vpermt2b ymm2, ymm6, yword [eax+64]
	vpermt2b zmm4, zmm3, zword [eax+1]
	vpermt2b zmm7, zmm5, zword [eax+64]
	vpermt2b xmm3, xmm5, [0x373]
	vpermt2b ymm4, ymm3, [0xf76]
	vpermt2b zmm0, zmm3, [0x4fb]
