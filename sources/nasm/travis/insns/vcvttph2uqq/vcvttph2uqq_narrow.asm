	vcvttph2uqq xmm6, xmm2
	vcvttph2uqq xmm5, dword [0x9ce]
	vcvttph2uqq ymm7, xmm2
	vcvttph2uqq ymm0, xmm5
	vcvttph2uqq zmm0, xmm1
	vcvttph2uqq zmm0, xmm1

%ifdef ERROR
	vcvttph2uqq xmm11, xmm12
	vcvttph2uqq ymm10, xmm13
	vcvttph2uqq zmm8, xmm15
	vcvttph2uqq xmm24, xmm30
	vcvttph2uqq ymm17, xmm22
	vcvttph2uqq zmm17, xmm22
%endif
