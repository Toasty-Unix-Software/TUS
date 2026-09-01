default rel
	vpunpckldq xmm5, xmm3, xmm6
	vpunpckldq xmm5, xmm6
	vpunpckldq xmm6, xmm6, xmm4
	vpunpckldq ymm6, ymm7, yword [0xbcb]
	vpunpckldq ymm6, yword [0xbcb]
	vpunpckldq ymm3, ymm2, ymm0
	vpunpckldq xmm1, xmm0, oword [0xf64]
	vpunpckldq xmm1, oword [0xf64]
	vpunpckldq xmm6, xmm2, oword [0x877]
	vpunpckldq ymm4, ymm6, yword [0x21f]
	vpunpckldq ymm4, yword [0x21f]
	vpunpckldq ymm7, ymm6, ymm0
	vpunpckldq xmm11, xmm9, xmm15
	vpunpckldq ymm14, ymm12, ymm13
	vpunpckldq xmm9, xmm9, xmm12
	vpunpckldq ymm10, ymm11, ymm11
	vpunpckldq xmm17, xmm22, xmm30
	vpunpckldq ymm30, ymm26, ymm21
	vpunpckldq xmm18, xmm21, xmm24
	vpunpckldq ymm29, ymm28, ymm28
	vpunpckldq xmm0{k4}, xmm5, oword [0x601]
	vpunpckldq ymm6{k5}, ymm6, ymm4
	vpunpckldq zmm3{k3}, zmm7, zword [0xce5]
	vpunpckldq xmm2{k4}{z}, xmm5, xmm6
	vpunpckldq ymm6{k4}{z}, ymm6, ymm5
	vpunpckldq zmm7{k7}{z}, zmm0, zword [0x1b8]
	vpunpckldq xmm5, xmm5, oword [eax+1]
	vpunpckldq xmm3, xmm6, oword [eax+64]
	vpunpckldq ymm0, ymm3, yword [eax+1]
	vpunpckldq ymm1, ymm0, yword [eax+64]
	vpunpckldq xmm0, xmm6, oword [eax+1]
	vpunpckldq xmm3, xmm5, oword [eax+64]
	vpunpckldq ymm2, ymm2, yword [eax+1]
	vpunpckldq ymm7, ymm3, yword [eax+64]
	vpunpckldq zmm5, zmm3, zword [eax+1]
	vpunpckldq zmm2, zmm7, zword [eax+64]
	vpunpckldq xmm6, xmm5, [0x7bc]
	vpunpckldq ymm0, ymm5, [0xe4b]
	vpunpckldq xmm7, xmm0, [0xc27]
	vpunpckldq ymm3, ymm1, [0x1ab]
	vpunpckldq zmm3, zmm5, [0xfc4]
