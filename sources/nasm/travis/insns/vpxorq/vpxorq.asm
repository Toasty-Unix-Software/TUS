default rel
	vpxorq xmm2, xmm0, xmm7
	vpxorq xmm2, xmm7
	vpxorq xmm1, xmm2, xmm7
	vpxorq ymm7, ymm2, ymm4
	vpxorq ymm7, ymm4
	vpxorq ymm6, ymm3, yword [0x818]
	vpxorq zmm2, zmm5, zword [0x11a]
	vpxorq zmm2, zword [0x11a]
	vpxorq zmm0, zmm5, zmm5
	vpxorq xmm15, xmm10, xmm12
	vpxorq ymm15, ymm13, ymm11
	vpxorq zmm11, zmm11, zmm15
	vpxorq xmm19, xmm30, xmm16
	vpxorq ymm27, ymm30, ymm23
	vpxorq zmm21, zmm30, zmm31
	vpxorq xmm1{k4}, xmm7, xmm5
	vpxorq ymm2{k1}, ymm3, yword [0x8ea]
	vpxorq zmm1{k7}, zmm1, zmm0
	vpxorq xmm0{k6}{z}, xmm6, xmm5
	vpxorq ymm2{k2}{z}, ymm3, ymm5
	vpxorq zmm3{k5}{z}, zmm7, zword [0x78c]
	vpxorq xmm4, xmm6, oword [eax+1]
	vpxorq xmm3, xmm3, oword [eax+64]
	vpxorq ymm4, ymm3, yword [eax+1]
	vpxorq ymm6, ymm5, yword [eax+64]
	vpxorq zmm2, zmm2, zword [eax+1]
	vpxorq zmm2, zmm2, zword [eax+64]
	vpxorq xmm6, xmm1, [0x44a]
	vpxorq ymm4, ymm5, [0xc6c]
	vpxorq zmm7, zmm2, [0x1fb]
