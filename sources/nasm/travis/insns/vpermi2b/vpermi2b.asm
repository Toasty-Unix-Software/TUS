default rel
	vpermi2b xmm6, xmm3, oword [0x29f]
	vpermi2b xmm6, xmm1, xmm7
	vpermi2b ymm7, ymm2, ymm0
	vpermi2b ymm6, ymm3, ymm6
	vpermi2b zmm6, zmm3, zmm5
	vpermi2b zmm1, zmm4, zword [0xe3a]
	vpermi2b xmm14, xmm14, xmm10
	vpermi2b ymm15, ymm11, ymm9
	vpermi2b zmm8, zmm12, zmm10
	vpermi2b xmm23, xmm17, xmm30
	vpermi2b ymm22, ymm23, ymm23
	vpermi2b zmm31, zmm26, zmm30
	vpermi2b xmm0{k1}, xmm0, oword [0x919]
	vpermi2b ymm3{k2}, ymm5, ymm6
	vpermi2b zmm3{k5}, zmm6, zmm6
	vpermi2b xmm3{k1}{z}, xmm0, xmm0
	vpermi2b ymm2{k2}{z}, ymm3, ymm1
	vpermi2b zmm0{k1}{z}, zmm0, zmm5
	vpermi2b xmm6, xmm2, oword [eax+1]
	vpermi2b xmm7, xmm6, oword [eax+64]
	vpermi2b ymm0, ymm4, yword [eax+1]
	vpermi2b ymm2, ymm5, yword [eax+64]
	vpermi2b zmm3, zmm5, zword [eax+1]
	vpermi2b zmm6, zmm7, zword [eax+64]
	vpermi2b xmm4, xmm3, [0x5ce]
	vpermi2b ymm1, ymm5, [0xa36]
	vpermi2b zmm7, zmm5, [0xc22]
