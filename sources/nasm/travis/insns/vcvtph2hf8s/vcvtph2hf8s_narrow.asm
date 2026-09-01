	vcvtph2hf8s xmm6, xmm3
	vcvtph2hf8s xmm5, oword [0xa4e]
	vcvtph2hf8s xmm1, ymm7
	vcvtph2hf8s xmm1, ymm3
	vcvtph2hf8s ymm4, zmm7
	vcvtph2hf8s ymm1, zmm0

%ifdef ERROR
	vcvtph2hf8s xmm12, xmm8
	vcvtph2hf8s xmm9, ymm9
	vcvtph2hf8s ymm15, zmm8
	vcvtph2hf8s xmm28, xmm31
	vcvtph2hf8s xmm24, ymm18
	vcvtph2hf8s ymm20, zmm18
%endif
