	vpmullq xmm3, xmm7, xmm3
	vpmullq xmm3, xmm3
	vpmullq xmm5, xmm2, oword [0xde4]
	vpmullq ymm1, ymm0, yword [0x15c]
	vpmullq ymm1, yword [0x15c]
	vpmullq ymm7, ymm2, ymm7
	vpmullq zmm7, zmm2, zmm0
	vpmullq zmm7, zmm0
	vpmullq zmm4, zmm7, zmm0

%ifdef ERROR
	vpmullq xmm15, xmm12, xmm10
	vpmullq ymm15, ymm8, ymm10
	vpmullq zmm14, zmm15, zmm12
	vpmullq xmm25, xmm22, xmm28
	vpmullq ymm26, ymm29, ymm18
	vpmullq zmm22, zmm24, zmm22
%endif
