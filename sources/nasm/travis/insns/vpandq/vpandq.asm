default rel
	vpandq xmm3, xmm0, oword [0x9e9]
	vpandq xmm3, oword [0x9e9]
	vpandq xmm7, xmm6, xmm0
	vpandq ymm2, ymm5, yword [0x23e]
	vpandq ymm2, yword [0x23e]
	vpandq ymm2, ymm5, ymm1
	vpandq zmm2, zmm5, zmm0
	vpandq zmm2, zmm0
	vpandq zmm6, zmm0, zmm4
	vpandq xmm10, xmm10, xmm14
	vpandq ymm8, ymm15, ymm15
	vpandq zmm14, zmm10, zmm11
	vpandq xmm16, xmm28, xmm26
	vpandq ymm19, ymm24, ymm26
	vpandq zmm16, zmm26, zmm25
	vpandq xmm5{k1}, xmm0, oword [0x207]
	vpandq ymm4{k1}, ymm5, ymm3
	vpandq zmm0{k1}, zmm5, zword [0x73f]
	vpandq xmm0{k5}{z}, xmm3, xmm4
	vpandq ymm3{k3}{z}, ymm0, ymm7
	vpandq zmm2{k6}{z}, zmm0, zmm6
	vpandq xmm7, xmm1, oword [eax+1]
	vpandq xmm4, xmm6, oword [eax+64]
	vpandq ymm0, ymm1, yword [eax+1]
	vpandq ymm2, ymm5, yword [eax+64]
	vpandq zmm5, zmm0, zword [eax+1]
	vpandq zmm6, zmm7, zword [eax+64]
	vpandq xmm7, xmm3, [0xf1a]
	vpandq ymm6, ymm1, [0x4d9]
	vpandq zmm0, zmm3, [0x5bc]
