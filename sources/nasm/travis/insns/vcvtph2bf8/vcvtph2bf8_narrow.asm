	vcvtph2bf8 xmm5, oword [0x6fc]
	vcvtph2bf8 xmm2, oword [0x4f5]
	vcvtph2bf8 xmm4, yword [0xc9e]
	vcvtph2bf8 xmm2, yword [0xf77]
	vcvtph2bf8 ymm0, zmm3
	vcvtph2bf8 ymm1, zmm0

%ifdef ERROR
	vcvtph2bf8 xmm8, xmm12
	vcvtph2bf8 xmm13, ymm12
	vcvtph2bf8 ymm15, zmm14
	vcvtph2bf8 xmm27, xmm17
	vcvtph2bf8 xmm31, ymm20
	vcvtph2bf8 ymm29, zmm26
%endif
