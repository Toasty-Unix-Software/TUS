default rel
	vpblendmb xmm4, xmm0, oword [0x7ff]
	vpblendmb xmm5, xmm7, xmm6
	vpblendmb ymm1, ymm1, yword [0xcca]
	vpblendmb ymm1, ymm6, ymm7
	vpblendmb zmm6, zmm4, zmm7
	vpblendmb zmm5, zmm3, zmm0
	vpblendmb xmm11, xmm11, xmm10
	vpblendmb ymm8, ymm14, ymm13
	vpblendmb zmm9, zmm14, zmm9
	vpblendmb xmm27, xmm17, xmm29
	vpblendmb ymm25, ymm29, ymm18
	vpblendmb zmm16, zmm21, zmm24
	vpblendmb xmm0{k4}, xmm1, oword [0xcfc]
	vpblendmb ymm2{k6}, ymm2, ymm1
	vpblendmb zmm6{k3}, zmm0, zword [0x483]
	vpblendmb xmm4{k1}{z}, xmm1, xmm0
	vpblendmb ymm0{k4}{z}, ymm2, ymm7
	vpblendmb zmm0{k6}{z}, zmm5, zword [0x7ea]
	vpblendmb xmm3, xmm7, oword [eax+1]
	vpblendmb xmm6, xmm0, oword [eax+64]
	vpblendmb ymm5, ymm3, yword [eax+1]
	vpblendmb ymm1, ymm4, yword [eax+64]
	vpblendmb zmm1, zmm1, zword [eax+1]
	vpblendmb zmm1, zmm1, zword [eax+64]
	vpblendmb xmm6, xmm0, [0xba0]
	vpblendmb ymm7, ymm1, [0xb9e]
	vpblendmb zmm1, zmm2, [0x34b]
