default rel
	vpmullq xmm3, xmm7, xmm3
	vpmullq xmm3, xmm3
	vpmullq xmm5, xmm2, oword [0xde4]
	vpmullq ymm1, ymm0, yword [0x15c]
	vpmullq ymm1, yword [0x15c]
	vpmullq ymm7, ymm2, ymm7
	vpmullq zmm7, zmm2, zmm0
	vpmullq zmm7, zmm0
	vpmullq zmm4, zmm7, zmm0
	vpmullq xmm15, xmm12, xmm10
	vpmullq ymm15, ymm8, ymm10
	vpmullq zmm14, zmm15, zmm12
	vpmullq xmm25, xmm22, xmm28
	vpmullq ymm26, ymm29, ymm18
	vpmullq zmm22, zmm24, zmm22
	vpmullq xmm4{k4}, xmm5, xmm1
	vpmullq ymm6{k4}, ymm5, ymm5
	vpmullq zmm2{k3}, zmm7, zmm7
	vpmullq xmm3{k6}{z}, xmm7, xmm3
	vpmullq ymm5{k1}{z}, ymm6, yword [0x3da]
	vpmullq zmm7{k5}{z}, zmm0, zmm1
	vpmullq xmm0, xmm6, oword [eax+1]
	vpmullq xmm5, xmm5, oword [eax+64]
	vpmullq ymm2, ymm5, yword [eax+1]
	vpmullq ymm3, ymm3, yword [eax+64]
	vpmullq zmm6, zmm1, zword [eax+1]
	vpmullq zmm0, zmm6, zword [eax+64]
	vpmullq xmm0, xmm5, [0xeed]
	vpmullq ymm1, ymm2, [0xb3e]
	vpmullq zmm0, zmm2, [0x149]
