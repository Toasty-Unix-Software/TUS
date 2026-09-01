default rel
	vpshufb xmm4, xmm4, xmm3
	vpshufb xmm4, xmm3
	vpshufb xmm3, xmm0, xmm4
	vpshufb ymm3, ymm3, ymm0
	vpshufb ymm3, ymm0
	vpshufb ymm0, ymm0, yword [0xee1]
	vpshufb xmm0, xmm2, oword [0xf56]
	vpshufb xmm0, oword [0xf56]
	vpshufb xmm3, xmm4, xmm1
	vpshufb ymm1, ymm2, yword [0x2bf]
	vpshufb ymm1, yword [0x2bf]
	vpshufb ymm2, ymm7, ymm4
	vpshufb xmm15, xmm10, xmm10
	vpshufb ymm8, ymm13, ymm12
	vpshufb xmm10, xmm12, xmm15
	vpshufb ymm10, ymm12, ymm15
	vpshufb xmm21, xmm18, xmm22
	vpshufb ymm29, ymm31, ymm26
	vpshufb xmm24, xmm19, xmm24
	vpshufb ymm23, ymm31, ymm31
	vpshufb xmm0{k2}, xmm3, xmm7
	vpshufb ymm5{k5}, ymm2, ymm4
	vpshufb zmm1{k3}, zmm5, zword [0xdee]
	vpshufb xmm0{k4}{z}, xmm1, xmm6
	vpshufb ymm0{k1}{z}, ymm7, ymm2
	vpshufb zmm2{k7}{z}, zmm3, zmm0
	vpshufb xmm7, xmm1, oword [eax+1]
	vpshufb xmm1, xmm3, oword [eax+64]
	vpshufb ymm6, ymm6, yword [eax+1]
	vpshufb ymm3, ymm5, yword [eax+64]
	vpshufb xmm7, xmm0, oword [eax+1]
	vpshufb xmm5, xmm0, oword [eax+64]
	vpshufb ymm3, ymm4, yword [eax+1]
	vpshufb ymm1, ymm1, yword [eax+64]
	vpshufb zmm1, zmm6, zword [eax+1]
	vpshufb zmm7, zmm5, zword [eax+64]
	vpshufb xmm7, xmm1, [0x5a5]
	vpshufb ymm0, ymm4, [0xe1b]
	vpshufb xmm2, xmm3, [0x2dc]
	vpshufb ymm0, ymm4, [0xf75]
	vpshufb zmm6, zmm7, [0x256]
