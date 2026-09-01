default rel
	vpmaxsw xmm5, xmm0, oword [0xd7e]
	vpmaxsw xmm5, oword [0xd7e]
	vpmaxsw xmm4, xmm4, xmm2
	vpmaxsw ymm0, ymm6, yword [0x1ac]
	vpmaxsw ymm0, yword [0x1ac]
	vpmaxsw ymm5, ymm7, ymm4
	vpmaxsw xmm7, xmm4, xmm4
	vpmaxsw xmm7, xmm4
	vpmaxsw xmm2, xmm3, oword [0x389]
	vpmaxsw ymm5, ymm0, yword [0x39b]
	vpmaxsw ymm5, yword [0x39b]
	vpmaxsw ymm3, ymm0, yword [0x9de]
	vpmaxsw xmm13, xmm12, xmm12
	vpmaxsw ymm10, ymm12, ymm12
	vpmaxsw xmm14, xmm13, xmm12
	vpmaxsw ymm11, ymm15, ymm15
	vpmaxsw xmm27, xmm29, xmm17
	vpmaxsw ymm23, ymm25, ymm19
	vpmaxsw xmm28, xmm18, xmm21
	vpmaxsw ymm26, ymm25, ymm19
	vpmaxsw xmm2{k1}, xmm1, xmm1
	vpmaxsw ymm0{k4}, ymm3, ymm6
	vpmaxsw zmm7{k1}, zmm3, zword [0x44a]
	vpmaxsw xmm6{k2}{z}, xmm0, xmm0
	vpmaxsw ymm2{k3}{z}, ymm0, ymm7
	vpmaxsw zmm6{k6}{z}, zmm6, zmm0
	vpmaxsw xmm3, xmm5, oword [eax+1]
	vpmaxsw xmm2, xmm7, oword [eax+64]
	vpmaxsw ymm1, ymm2, yword [eax+1]
	vpmaxsw ymm1, ymm7, yword [eax+64]
	vpmaxsw xmm7, xmm0, oword [eax+1]
	vpmaxsw xmm4, xmm4, oword [eax+64]
	vpmaxsw ymm0, ymm5, yword [eax+1]
	vpmaxsw ymm2, ymm1, yword [eax+64]
	vpmaxsw zmm2, zmm0, zword [eax+1]
	vpmaxsw zmm7, zmm0, zword [eax+64]
	vpmaxsw xmm6, xmm4, [0x43d]
	vpmaxsw ymm0, ymm5, [0x3c9]
	vpmaxsw xmm0, xmm6, [0x6e5]
	vpmaxsw ymm3, ymm4, [0x3d3]
	vpmaxsw zmm4, zmm1, [0x1cb]
