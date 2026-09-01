	vcvttph2qq xmm5, xmm4
	vcvttph2qq xmm3, xmm6
	vcvttph2qq ymm4, xmm1
	vcvttph2qq ymm3, xmm4
	vcvttph2qq zmm6, xmm6
	vcvttph2qq zmm5, oword [0x992]

%ifdef ERROR
	vcvttph2qq xmm11, xmm9
	vcvttph2qq ymm12, xmm13
	vcvttph2qq zmm13, xmm13
	vcvttph2qq xmm17, xmm16
	vcvttph2qq ymm22, xmm24
	vcvttph2qq zmm23, xmm29
%endif
