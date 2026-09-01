default rel
	vpmaxuw xmm1, xmm0, xmm3
	vpmaxuw xmm1, xmm3
	vpmaxuw xmm6, xmm0, oword [0xbfa]
	vpmaxuw ymm3, ymm6, ymm7
	vpmaxuw ymm3, ymm7
	vpmaxuw ymm0, ymm3, yword [0x37f]
	vpmaxuw xmm3, xmm3, xmm7
	vpmaxuw xmm3, xmm7
	vpmaxuw xmm2, xmm5, xmm1
	vpmaxuw ymm0, ymm6, ymm6
	vpmaxuw ymm0, ymm6
	vpmaxuw ymm3, ymm7, ymm3
	vpmaxuw xmm11, xmm10, xmm15
	vpmaxuw ymm15, ymm12, ymm10
	vpmaxuw xmm12, xmm15, xmm12
	vpmaxuw ymm15, ymm9, ymm15
	vpmaxuw xmm19, xmm27, xmm24
	vpmaxuw ymm26, ymm18, ymm28
	vpmaxuw xmm17, xmm21, xmm25
	vpmaxuw ymm27, ymm24, ymm26
	vpmaxuw xmm6{k2}, xmm7, xmm2
	vpmaxuw ymm0{k1}, ymm7, ymm4
	vpmaxuw zmm1{k3}, zmm3, zmm4
	vpmaxuw xmm4{k7}{z}, xmm4, oword [0xa9e]
	vpmaxuw ymm0{k1}{z}, ymm3, ymm1
	vpmaxuw zmm3{k4}{z}, zmm7, zmm5
	vpmaxuw xmm2, xmm0, oword [eax+1]
	vpmaxuw xmm2, xmm1, oword [eax+64]
	vpmaxuw ymm2, ymm0, yword [eax+1]
	vpmaxuw ymm5, ymm5, yword [eax+64]
	vpmaxuw xmm3, xmm5, oword [eax+1]
	vpmaxuw xmm1, xmm4, oword [eax+64]
	vpmaxuw ymm0, ymm7, yword [eax+1]
	vpmaxuw ymm0, ymm6, yword [eax+64]
	vpmaxuw zmm5, zmm4, zword [eax+1]
	vpmaxuw zmm5, zmm2, zword [eax+64]
	vpmaxuw xmm0, xmm0, [0x258]
	vpmaxuw ymm2, ymm4, [0x35f]
	vpmaxuw xmm5, xmm0, [0x89c]
	vpmaxuw ymm1, ymm1, [0x881]
	vpmaxuw zmm7, zmm0, [0x8b0]
