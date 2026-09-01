	vcvtbiasph2hf8s xmm5, xmm6, xmm4
	vcvtbiasph2hf8s xmm3, xmm0, xmm7
	vcvtbiasph2hf8s xmm2, ymm0, yword [0x350]
	vcvtbiasph2hf8s xmm6, ymm5, ymm1
	vcvtbiasph2hf8s ymm0, zmm0, zmm7
	vcvtbiasph2hf8s ymm7, zmm3, zmm6

%ifdef ERROR
	vcvtbiasph2hf8s xmm12, xmm9, xmm12
	vcvtbiasph2hf8s xmm8, ymm13, ymm8
	vcvtbiasph2hf8s ymm8, zmm10, zmm10
	vcvtbiasph2hf8s xmm26, xmm17, xmm24
	vcvtbiasph2hf8s xmm23, ymm22, ymm27
	vcvtbiasph2hf8s ymm18, zmm22, zmm31
%endif
