	vcvtdq2ps xmm4, oword [0x244]
	vcvtdq2ps xmm5, xmm1
	vcvtdq2ps ymm4, yword [0xa67]
	vcvtdq2ps ymm4, ymm5
	vcvtdq2ps xmm4, xmm1
	vcvtdq2ps xmm3, xmm1
	vcvtdq2ps ymm1, yword [0xb14]
	vcvtdq2ps ymm6, yword [0xb57]

%ifdef ERROR
	vcvtdq2ps xmm10, xmm13
	vcvtdq2ps ymm8, ymm11
	vcvtdq2ps xmm11, xmm14
	vcvtdq2ps ymm9, ymm9
	vcvtdq2ps xmm24, xmm28
	vcvtdq2ps ymm29, ymm28
	vcvtdq2ps xmm26, xmm29
	vcvtdq2ps ymm25, ymm22
%endif
