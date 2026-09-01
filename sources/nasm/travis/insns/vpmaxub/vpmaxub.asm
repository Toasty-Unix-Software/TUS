default rel
	vpmaxub xmm4, xmm0, xmm2
	vpmaxub xmm4, xmm2
	vpmaxub xmm5, xmm5, oword [0xd2b]
	vpmaxub ymm5, ymm1, yword [0xacd]
	vpmaxub ymm5, yword [0xacd]
	vpmaxub ymm5, ymm7, ymm0
	vpmaxub xmm7, xmm4, oword [0x4f3]
	vpmaxub xmm7, oword [0x4f3]
	vpmaxub xmm4, xmm5, xmm4
	vpmaxub ymm6, ymm1, yword [0xb06]
	vpmaxub ymm6, yword [0xb06]
	vpmaxub ymm7, ymm7, ymm3
	vpmaxub xmm8, xmm10, xmm15
	vpmaxub ymm13, ymm11, ymm10
	vpmaxub xmm8, xmm14, xmm11
	vpmaxub ymm10, ymm11, ymm12
	vpmaxub xmm25, xmm30, xmm30
	vpmaxub ymm19, ymm26, ymm25
	vpmaxub xmm27, xmm31, xmm16
	vpmaxub ymm29, ymm26, ymm21
	vpmaxub xmm7{k3}, xmm5, oword [0x3ae]
	vpmaxub ymm2{k2}, ymm7, ymm7
	vpmaxub zmm0{k5}, zmm5, zmm5
	vpmaxub xmm7{k4}{z}, xmm7, xmm4
	vpmaxub ymm4{k5}{z}, ymm0, ymm2
	vpmaxub zmm7{k7}{z}, zmm1, zmm6
	vpmaxub xmm5, xmm5, oword [eax+1]
	vpmaxub xmm3, xmm5, oword [eax+64]
	vpmaxub ymm1, ymm3, yword [eax+1]
	vpmaxub ymm3, ymm0, yword [eax+64]
	vpmaxub xmm6, xmm3, oword [eax+1]
	vpmaxub xmm4, xmm7, oword [eax+64]
	vpmaxub ymm5, ymm5, yword [eax+1]
	vpmaxub ymm3, ymm5, yword [eax+64]
	vpmaxub zmm7, zmm5, zword [eax+1]
	vpmaxub zmm2, zmm3, zword [eax+64]
	vpmaxub xmm1, xmm4, [0x9e4]
	vpmaxub ymm6, ymm6, [0x557]
	vpmaxub xmm7, xmm7, [0xd9e]
	vpmaxub ymm4, ymm1, [0xf55]
	vpmaxub zmm2, zmm3, [0x8c5]
