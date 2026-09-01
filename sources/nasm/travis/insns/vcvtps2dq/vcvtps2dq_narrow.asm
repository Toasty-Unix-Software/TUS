	vcvtps2dq xmm0, xmm3
	vcvtps2dq xmm4, xmm1
	vcvtps2dq ymm5, yword [0x798]
	vcvtps2dq ymm1, ymm6
	vcvtps2dq xmm3, oword [0x983]
	vcvtps2dq xmm3, oword [0x298]
	vcvtps2dq ymm6, yword [0x84d]
	vcvtps2dq ymm6, ymm6

%ifdef ERROR
	vcvtps2dq xmm11, xmm15
	vcvtps2dq ymm14, ymm15
	vcvtps2dq xmm14, xmm10
	vcvtps2dq ymm12, ymm8
	vcvtps2dq xmm16, xmm28
	vcvtps2dq ymm31, ymm20
	vcvtps2dq xmm25, xmm31
	vcvtps2dq ymm23, ymm28
%endif
