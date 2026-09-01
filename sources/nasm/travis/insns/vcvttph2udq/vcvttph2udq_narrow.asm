	vcvttph2udq xmm7, qword [0x96e]
	vcvttph2udq xmm4, xmm3
	vcvttph2udq ymm0, xmm2
	vcvttph2udq ymm3, oword [0x858]
	vcvttph2udq zmm6, ymm3
	vcvttph2udq zmm7, yword [0xc25]

%ifdef ERROR
	vcvttph2udq xmm15, xmm15
	vcvttph2udq ymm14, xmm12
	vcvttph2udq zmm11, ymm10
	vcvttph2udq xmm29, xmm17
	vcvttph2udq ymm24, xmm26
	vcvttph2udq zmm27, ymm22
%endif
