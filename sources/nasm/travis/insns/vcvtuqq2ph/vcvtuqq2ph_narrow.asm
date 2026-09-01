	vcvtuqq2ph xmm1, xmm7
	vcvtuqq2ph xmm4, xmm6
	vcvtuqq2ph xmm1, yword [0x163]
	vcvtuqq2ph xmm3, yword [0x127]
	vcvtuqq2ph xmm6, zmm1
	vcvtuqq2ph xmm6, zmm2

%ifdef ERROR
	vcvtuqq2ph xmm8, xmm11
	vcvtuqq2ph xmm15, ymm10
	vcvtuqq2ph xmm10, zmm8
	vcvtuqq2ph xmm17, xmm27
	vcvtuqq2ph xmm19, ymm26
	vcvtuqq2ph xmm17, zmm28
%endif
