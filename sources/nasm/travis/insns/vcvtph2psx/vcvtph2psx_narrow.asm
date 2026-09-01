	vcvtph2psx xmm2, qword [0x3d6]
	vcvtph2psx xmm7, xmm2
	vcvtph2psx ymm2, oword [0xe0c]
	vcvtph2psx ymm1, xmm5
	vcvtph2psx zmm6, ymm0
	vcvtph2psx zmm3, ymm7

%ifdef ERROR
	vcvtph2psx xmm9, xmm14
	vcvtph2psx ymm13, xmm15
	vcvtph2psx zmm15, ymm12
	vcvtph2psx xmm29, xmm29
	vcvtph2psx ymm31, xmm16
	vcvtph2psx zmm28, ymm28
%endif
