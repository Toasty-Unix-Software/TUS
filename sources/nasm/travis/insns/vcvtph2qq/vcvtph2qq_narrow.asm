	vcvtph2qq xmm5, xmm1
	vcvtph2qq xmm2, xmm1
	vcvtph2qq ymm6, qword [0x8b1]
	vcvtph2qq ymm7, xmm1
	vcvtph2qq zmm3, oword [0x195]
	vcvtph2qq zmm2, oword [0x378]

%ifdef ERROR
	vcvtph2qq xmm13, xmm15
	vcvtph2qq ymm9, xmm9
	vcvtph2qq zmm12, xmm11
	vcvtph2qq xmm20, xmm22
	vcvtph2qq ymm22, xmm29
	vcvtph2qq zmm30, xmm31
%endif
