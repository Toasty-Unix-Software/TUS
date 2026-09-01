default rel
	vpsubq xmm5, xmm5, xmm0
	vpsubq xmm5, xmm0
	vpsubq xmm7, xmm3, oword [0x829]
	vpsubq ymm5, ymm7, ymm2
	vpsubq ymm5, ymm2
	vpsubq ymm7, ymm7, ymm3
	vpsubq xmm0, xmm1, oword [0xc55]
	vpsubq xmm0, oword [0xc55]
	vpsubq xmm2, xmm4, xmm6
	vpsubq ymm4, ymm4, ymm1
	vpsubq ymm4, ymm1
	vpsubq ymm2, ymm3, ymm1
	vpsubq xmm15, xmm8, xmm13
	vpsubq ymm14, ymm11, ymm11
	vpsubq xmm13, xmm10, xmm10
	vpsubq ymm15, ymm8, ymm14
	vpsubq xmm16, xmm28, xmm30
	vpsubq ymm25, ymm31, ymm29
	vpsubq xmm26, xmm22, xmm20
	vpsubq ymm28, ymm30, ymm23
	vpsubq xmm1{k5}, xmm3, oword [0x6f1]
	vpsubq ymm6{k7}, ymm3, yword [0x8f1]
	vpsubq zmm3{k4}, zmm4, zmm0
	vpsubq xmm1{k6}{z}, xmm3, oword [0xa04]
	vpsubq ymm3{k6}{z}, ymm6, ymm0
	vpsubq zmm7{k6}{z}, zmm5, zmm0
	vpsubq xmm5, xmm4, oword [eax+1]
	vpsubq xmm7, xmm3, oword [eax+64]
	vpsubq ymm3, ymm6, yword [eax+1]
	vpsubq ymm1, ymm2, yword [eax+64]
	vpsubq xmm4, xmm2, oword [eax+1]
	vpsubq xmm4, xmm4, oword [eax+64]
	vpsubq ymm2, ymm5, yword [eax+1]
	vpsubq ymm0, ymm0, yword [eax+64]
	vpsubq zmm2, zmm7, zword [eax+1]
	vpsubq zmm5, zmm2, zword [eax+64]
	vpsubq xmm4, xmm0, [0x94d]
	vpsubq ymm5, ymm0, [0xb07]
	vpsubq xmm6, xmm1, [0x204]
	vpsubq ymm5, ymm6, [0x2d1]
	vpsubq zmm2, zmm7, [0xc35]
