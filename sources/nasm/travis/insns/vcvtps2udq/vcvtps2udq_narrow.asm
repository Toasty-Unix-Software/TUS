	vcvtps2udq xmm4, xmm2
	vcvtps2udq xmm2, xmm7
	vcvtps2udq ymm3, ymm1
	vcvtps2udq ymm5, ymm0
	vcvtps2udq zmm7, zmm6
	vcvtps2udq zmm7, zword [0x5bb]

%ifdef ERROR
	vcvtps2udq xmm10, xmm14
	vcvtps2udq ymm8, ymm13
	vcvtps2udq zmm15, zmm14
	vcvtps2udq xmm26, xmm18
	vcvtps2udq ymm28, ymm17
	vcvtps2udq zmm21, zmm22
%endif
