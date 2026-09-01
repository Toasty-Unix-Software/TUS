	vcvtbiasph2hf8 xmm3, xmm1, xmm3
	vcvtbiasph2hf8 xmm2, xmm7, xmm5
	vcvtbiasph2hf8 xmm0, ymm1, ymm4
	vcvtbiasph2hf8 xmm4, ymm2, ymm0
	vcvtbiasph2hf8 ymm2, zmm3, zmm5
	vcvtbiasph2hf8 ymm4, zmm3, zmm4

%ifdef ERROR
	vcvtbiasph2hf8 xmm15, xmm14, xmm10
	vcvtbiasph2hf8 xmm11, ymm14, ymm8
	vcvtbiasph2hf8 ymm11, zmm12, zmm15
	vcvtbiasph2hf8 xmm18, xmm25, xmm24
	vcvtbiasph2hf8 xmm28, ymm25, ymm24
	vcvtbiasph2hf8 ymm16, zmm26, zmm17
%endif
