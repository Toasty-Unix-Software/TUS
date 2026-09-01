default rel
	vpsubsb xmm3, xmm1, xmm1
	vpsubsb xmm3, xmm1
	vpsubsb xmm7, xmm3, xmm1
	vpsubsb ymm6, ymm1, ymm5
	vpsubsb ymm6, ymm5
	vpsubsb ymm0, ymm6, yword [0xa54]
	vpsubsb xmm3, xmm7, xmm2
	vpsubsb xmm3, xmm2
	vpsubsb xmm3, xmm6, xmm1
	vpsubsb ymm7, ymm4, ymm0
	vpsubsb ymm7, ymm0
	vpsubsb ymm4, ymm5, ymm2
	vpsubsb xmm10, xmm11, xmm15
	vpsubsb ymm15, ymm12, ymm12
	vpsubsb xmm9, xmm9, xmm8
	vpsubsb ymm15, ymm12, ymm8
	vpsubsb xmm28, xmm26, xmm30
	vpsubsb ymm30, ymm16, ymm30
	vpsubsb xmm25, xmm19, xmm17
	vpsubsb ymm17, ymm23, ymm25
	vpsubsb xmm4{k7}, xmm0, oword [0x241]
	vpsubsb ymm2{k4}, ymm5, ymm6
	vpsubsb zmm0{k6}, zmm7, zword [0x942]
	vpsubsb xmm0{k7}{z}, xmm6, xmm7
	vpsubsb ymm1{k5}{z}, ymm7, yword [0x528]
	vpsubsb zmm0{k5}{z}, zmm7, zmm4
	vpsubsb xmm4, xmm2, oword [eax+1]
	vpsubsb xmm5, xmm4, oword [eax+64]
	vpsubsb ymm4, ymm2, yword [eax+1]
	vpsubsb ymm3, ymm1, yword [eax+64]
	vpsubsb xmm0, xmm3, oword [eax+1]
	vpsubsb xmm2, xmm0, oword [eax+64]
	vpsubsb ymm1, ymm7, yword [eax+1]
	vpsubsb ymm6, ymm2, yword [eax+64]
	vpsubsb zmm1, zmm0, zword [eax+1]
	vpsubsb zmm6, zmm3, zword [eax+64]
	vpsubsb xmm3, xmm4, [0x3b0]
	vpsubsb ymm5, ymm3, [0x5e3]
	vpsubsb xmm1, xmm0, [0x2a9]
	vpsubsb ymm1, ymm1, [0xfb2]
	vpsubsb zmm2, zmm1, [0xa4c]
