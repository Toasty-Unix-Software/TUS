	vcvtph2hf8 xmm4, xmm0
	vcvtph2hf8 xmm6, xmm0
	vcvtph2hf8 xmm1, yword [0x82f]
	vcvtph2hf8 xmm6, ymm0
	vcvtph2hf8 ymm7, zword [0xe79]
	vcvtph2hf8 ymm7, zmm2

%ifdef ERROR
	vcvtph2hf8 xmm13, xmm10
	vcvtph2hf8 xmm10, ymm9
	vcvtph2hf8 ymm11, zmm14
	vcvtph2hf8 xmm29, xmm24
	vcvtph2hf8 xmm27, ymm19
	vcvtph2hf8 ymm21, zmm27
%endif
