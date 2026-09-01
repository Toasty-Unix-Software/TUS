default rel
	vpermb xmm3, xmm0, xmm7
	vpermb xmm3, xmm7
	vpermb xmm6, xmm0, oword [0x14b]
	vpermb ymm7, ymm4, ymm0
	vpermb ymm7, ymm0
	vpermb ymm1, ymm1, yword [0x469]
	vpermb zmm1, zmm4, zmm0
	vpermb zmm1, zmm0
	vpermb zmm4, zmm3, zmm3
	vpermb xmm11, xmm13, xmm10
	vpermb ymm8, ymm12, ymm11
	vpermb zmm10, zmm8, zmm15
	vpermb xmm24, xmm24, xmm16
	vpermb ymm31, ymm31, ymm31
	vpermb zmm25, zmm29, zmm22
	vpermb xmm4{k1}, xmm3, oword [0xfc8]
	vpermb ymm2{k5}, ymm4, ymm6
	vpermb zmm4{k6}, zmm1, zmm4
	vpermb xmm2{k2}{z}, xmm1, xmm1
	vpermb ymm7{k3}{z}, ymm2, ymm1
	vpermb zmm5{k6}{z}, zmm0, zmm7
	vpermb xmm3, xmm1, oword [eax+1]
	vpermb xmm6, xmm1, oword [eax+64]
	vpermb ymm6, ymm4, yword [eax+1]
	vpermb ymm2, ymm1, yword [eax+64]
	vpermb zmm0, zmm3, zword [eax+1]
	vpermb zmm0, zmm2, zword [eax+64]
	vpermb xmm7, xmm0, [0x980]
	vpermb ymm6, ymm5, [0xd45]
	vpermb zmm3, zmm1, [0x20e]
