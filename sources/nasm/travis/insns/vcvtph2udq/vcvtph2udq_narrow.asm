	vcvtph2udq xmm6, xmm2
	vcvtph2udq xmm7, xmm0
	vcvtph2udq ymm3, xmm2
	vcvtph2udq ymm3, xmm6
	vcvtph2udq zmm4, ymm0
	vcvtph2udq zmm3, yword [0xb0c]

%ifdef ERROR
	vcvtph2udq xmm10, xmm9
	vcvtph2udq ymm9, xmm14
	vcvtph2udq zmm14, ymm10
	vcvtph2udq xmm21, xmm31
	vcvtph2udq ymm18, xmm30
	vcvtph2udq zmm22, ymm17
%endif
