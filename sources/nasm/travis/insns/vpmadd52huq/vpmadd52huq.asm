default rel
	vpmadd52huq xmm2, xmm6, xmm4
	vpmadd52huq xmm4, xmm5, oword [0xa0b]
	vpmadd52huq ymm4, ymm7, yword [0xe8f]
	vpmadd52huq ymm7, ymm5, ymm0
	vpmadd52huq xmm6, xmm0, xmm4
	vpmadd52huq xmm2, xmm6, xmm4
	vpmadd52huq ymm1, ymm3, ymm7
	vpmadd52huq ymm0, ymm1, ymm7
	vpmadd52huq xmm9, xmm12, xmm10
	vpmadd52huq ymm14, ymm10, ymm10
	vpmadd52huq xmm12, xmm13, xmm10
	vpmadd52huq ymm12, ymm15, ymm8
	vpmadd52huq xmm23, xmm24, xmm24
	vpmadd52huq ymm20, ymm16, ymm17
	vpmadd52huq xmm22, xmm27, xmm31
	vpmadd52huq ymm28, ymm26, ymm25
	vpmadd52huq xmm2{k7}, xmm7, xmm5
	vpmadd52huq ymm0{k4}, ymm4, yword [0xb50]
	vpmadd52huq zmm4{k7}, zmm2, zmm7
	vpmadd52huq xmm4{k7}{z}, xmm1, xmm5
	vpmadd52huq ymm5{k1}{z}, ymm0, ymm1
	vpmadd52huq zmm0{k4}{z}, zmm2, zword [0x3ce]
	vpmadd52huq xmm3, xmm0, oword [eax+1]
	vpmadd52huq xmm6, xmm5, oword [eax+64]
	vpmadd52huq ymm5, ymm0, yword [eax+1]
	vpmadd52huq ymm1, ymm6, yword [eax+64]
	vpmadd52huq xmm1, xmm7, oword [eax+1]
	vpmadd52huq xmm3, xmm7, oword [eax+64]
	vpmadd52huq ymm3, ymm0, yword [eax+1]
	vpmadd52huq ymm4, ymm7, yword [eax+64]
	vpmadd52huq zmm4, zmm7, zword [eax+1]
	vpmadd52huq zmm1, zmm5, zword [eax+64]
	vpmadd52huq xmm7, xmm0, [0xa3c]
	vpmadd52huq ymm1, ymm0, [0x4f3]
	vpmadd52huq xmm7, xmm5, [0x848]
	vpmadd52huq ymm4, ymm6, [0x742]
	vpmadd52huq zmm2, zmm1, [0x9bc]
