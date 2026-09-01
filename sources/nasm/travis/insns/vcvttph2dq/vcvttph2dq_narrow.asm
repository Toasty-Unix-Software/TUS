	vcvttph2dq xmm5, xmm4
	vcvttph2dq xmm4, xmm6
	vcvttph2dq ymm6, xmm7
	vcvttph2dq ymm0, xmm7
	vcvttph2dq zmm0, yword [0x3df]
	vcvttph2dq zmm4, ymm7

%ifdef ERROR
	vcvttph2dq xmm15, xmm14
	vcvttph2dq ymm13, xmm13
	vcvttph2dq zmm10, ymm9
	vcvttph2dq xmm18, xmm17
	vcvttph2dq ymm20, xmm16
	vcvttph2dq zmm21, ymm27
%endif
