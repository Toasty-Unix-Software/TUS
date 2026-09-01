	vcvtph2dq xmm0, qword [0x276]
	vcvtph2dq xmm4, xmm4
	vcvtph2dq ymm5, oword [0xa32]
	vcvtph2dq ymm1, xmm6
	vcvtph2dq zmm6, ymm4
	vcvtph2dq zmm3, ymm1

%ifdef ERROR
	vcvtph2dq xmm8, xmm14
	vcvtph2dq ymm14, xmm14
	vcvtph2dq zmm10, ymm15
	vcvtph2dq xmm23, xmm31
	vcvtph2dq ymm19, xmm22
	vcvtph2dq zmm17, ymm27
%endif
