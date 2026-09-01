	vcvtph2uqq xmm7, dword [0xc25]
	vcvtph2uqq xmm6, xmm4
	vcvtph2uqq ymm1, xmm4
	vcvtph2uqq ymm0, xmm1
	vcvtph2uqq zmm4, oword [0xf00]
	vcvtph2uqq zmm5, oword [0x469]

%ifdef ERROR
	vcvtph2uqq xmm9, xmm10
	vcvtph2uqq ymm9, xmm10
	vcvtph2uqq zmm13, xmm8
	vcvtph2uqq xmm18, xmm19
	vcvtph2uqq ymm16, xmm28
	vcvtph2uqq zmm26, xmm26
%endif
