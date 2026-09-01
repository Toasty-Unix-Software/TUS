	vcvttps2uqq xmm6, xmm5
	vcvttps2uqq xmm4, xmm3
	vcvttps2uqq ymm6, xmm1
	vcvttps2uqq ymm3, xmm1
	vcvttps2uqq zmm2, ymm5
	vcvttps2uqq zmm6, ymm3

%ifdef ERROR
	vcvttps2uqq xmm9, xmm15
	vcvttps2uqq ymm15, xmm14
	vcvttps2uqq zmm8, ymm14
	vcvttps2uqq xmm31, xmm26
	vcvttps2uqq ymm19, xmm20
	vcvttps2uqq zmm16, ymm17
%endif
