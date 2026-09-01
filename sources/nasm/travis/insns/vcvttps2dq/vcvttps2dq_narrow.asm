	vcvttps2dq xmm2, xmm7
	vcvttps2dq xmm4, xmm5
	vcvttps2dq ymm7, ymm4
	vcvttps2dq ymm4, ymm6
	vcvttps2dq xmm1, oword [0xcc7]
	vcvttps2dq xmm4, xmm6
	vcvttps2dq ymm6, ymm2
	vcvttps2dq ymm4, ymm2

%ifdef ERROR
	vcvttps2dq xmm11, xmm9
	vcvttps2dq ymm12, ymm13
	vcvttps2dq xmm13, xmm13
	vcvttps2dq ymm14, ymm12
	vcvttps2dq xmm30, xmm26
	vcvttps2dq ymm23, ymm23
	vcvttps2dq xmm21, xmm16
	vcvttps2dq ymm25, ymm27
%endif
