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

%ifdef ERROR
	vpmaxub xmm8, xmm10, xmm15
	vpmaxub ymm13, ymm11, ymm10
	vpmaxub xmm8, xmm14, xmm11
	vpmaxub ymm10, ymm11, ymm12
	vpmaxub xmm25, xmm30, xmm30
	vpmaxub ymm19, ymm26, ymm25
	vpmaxub xmm27, xmm31, xmm16
	vpmaxub ymm29, ymm26, ymm21
%endif
