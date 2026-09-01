	vcvtph2ps ymm1, xmm3
	vcvtph2ps ymm5, xmm7
	vcvtph2ps xmm0, qword [0x1f6]
	vcvtph2ps xmm1, qword [0xe87]
	vcvtph2ps xmm3, xmm2
	vcvtph2ps xmm5, xmm6
	vcvtph2ps ymm3, xmm6
	vcvtph2ps ymm7, xmm7

%ifdef ERROR
	vcvtph2ps ymm11, xmm9
	vcvtph2ps xmm10, xmm14
	vcvtph2ps xmm8, xmm8
	vcvtph2ps ymm10, xmm10
	vcvtph2ps ymm30, xmm20
	vcvtph2ps xmm30, xmm17
	vcvtph2ps xmm22, xmm24
	vcvtph2ps ymm20, xmm22
%endif
