	vcvttph2w xmm1, oword [0xde1]
	vcvttph2w xmm6, oword [0x19c]
	vcvttph2w ymm5, ymm0
	vcvttph2w ymm7, ymm4
	vcvttph2w zmm2, zmm6
	vcvttph2w zmm7, zmm5

%ifdef ERROR
	vcvttph2w xmm11, xmm13
	vcvttph2w ymm8, ymm15
	vcvttph2w zmm13, zmm9
	vcvttph2w xmm20, xmm28
	vcvttph2w ymm20, ymm28
	vcvttph2w zmm18, zmm22
%endif
