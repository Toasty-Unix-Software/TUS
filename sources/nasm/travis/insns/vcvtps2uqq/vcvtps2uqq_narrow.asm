	vcvtps2uqq xmm7, xmm4
	vcvtps2uqq xmm2, xmm4
	vcvtps2uqq ymm7, xmm4
	vcvtps2uqq ymm7, xmm7
	vcvtps2uqq zmm0, ymm7
	vcvtps2uqq zmm5, ymm6

%ifdef ERROR
	vcvtps2uqq xmm11, xmm10
	vcvtps2uqq ymm14, xmm11
	vcvtps2uqq zmm12, ymm12
	vcvtps2uqq xmm30, xmm17
	vcvtps2uqq ymm17, xmm27
	vcvtps2uqq zmm16, ymm22
%endif
