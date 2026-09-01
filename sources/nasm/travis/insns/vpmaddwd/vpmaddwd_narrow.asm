	vpmaddwd xmm0, xmm1, xmm7
	vpmaddwd xmm0, xmm7
	vpmaddwd xmm6, xmm5, xmm7
	vpmaddwd ymm4, ymm6, ymm7
	vpmaddwd ymm4, ymm7
	vpmaddwd ymm5, ymm5, ymm6
	vpmaddwd xmm7, xmm6, xmm0
	vpmaddwd xmm7, xmm0
	vpmaddwd xmm1, xmm1, oword [0xc6e]
	vpmaddwd ymm0, ymm2, yword [0xc2b]
	vpmaddwd ymm0, yword [0xc2b]
	vpmaddwd ymm6, ymm6, ymm4

%ifdef ERROR
	vpmaddwd xmm9, xmm9, xmm13
	vpmaddwd ymm9, ymm10, ymm8
	vpmaddwd xmm14, xmm12, xmm10
	vpmaddwd ymm15, ymm13, ymm12
	vpmaddwd xmm21, xmm28, xmm23
	vpmaddwd ymm28, ymm21, ymm20
	vpmaddwd xmm16, xmm27, xmm22
	vpmaddwd ymm31, ymm29, ymm29
%endif
