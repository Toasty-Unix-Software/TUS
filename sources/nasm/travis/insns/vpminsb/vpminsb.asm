default rel
	vpminsb xmm2, xmm2, xmm7
	vpminsb xmm2, xmm7
	vpminsb xmm3, xmm0, xmm7
	vpminsb ymm7, ymm6, yword [0x59b]
	vpminsb ymm7, yword [0x59b]
	vpminsb ymm0, ymm2, yword [0xb11]
	vpminsb xmm2, xmm4, oword [0xbef]
	vpminsb xmm2, oword [0xbef]
	vpminsb xmm5, xmm2, xmm0
	vpminsb ymm7, ymm1, ymm7
	vpminsb ymm7, ymm7
	vpminsb ymm7, ymm5, yword [0xa35]
	vpminsb xmm10, xmm11, xmm12
	vpminsb ymm9, ymm9, ymm11
	vpminsb xmm11, xmm9, xmm8
	vpminsb ymm12, ymm12, ymm14
	vpminsb xmm28, xmm23, xmm28
	vpminsb ymm22, ymm28, ymm16
	vpminsb xmm18, xmm18, xmm30
	vpminsb ymm23, ymm29, ymm27
	vpminsb xmm3{k5}, xmm4, xmm3
	vpminsb ymm0{k6}, ymm7, ymm2
	vpminsb zmm4{k4}, zmm7, zmm1
	vpminsb xmm5{k6}{z}, xmm1, xmm7
	vpminsb ymm2{k3}{z}, ymm6, ymm2
	vpminsb zmm4{k3}{z}, zmm4, zmm7
	vpminsb xmm7, xmm4, oword [eax+1]
	vpminsb xmm7, xmm5, oword [eax+64]
	vpminsb ymm3, ymm6, yword [eax+1]
	vpminsb ymm3, ymm5, yword [eax+64]
	vpminsb xmm0, xmm5, oword [eax+1]
	vpminsb xmm2, xmm1, oword [eax+64]
	vpminsb ymm4, ymm2, yword [eax+1]
	vpminsb ymm7, ymm1, yword [eax+64]
	vpminsb zmm1, zmm0, zword [eax+1]
	vpminsb zmm6, zmm6, zword [eax+64]
	vpminsb xmm6, xmm0, [0xd7d]
	vpminsb ymm2, ymm3, [0x16d]
	vpminsb xmm0, xmm5, [0xc80]
	vpminsb ymm7, ymm7, [0xe64]
	vpminsb zmm1, zmm3, [0x758]
