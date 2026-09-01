	vcvtbiasph2bf8 xmm7, xmm4, oword [0x450]
	vcvtbiasph2bf8 xmm0, xmm1, oword [0x3bb]
	vcvtbiasph2bf8 xmm6, ymm2, ymm7
	vcvtbiasph2bf8 xmm4, ymm1, ymm7
	vcvtbiasph2bf8 ymm5, zmm0, zmm0
	vcvtbiasph2bf8 ymm4, zmm6, zmm4

%ifdef ERROR
	vcvtbiasph2bf8 xmm13, xmm15, xmm10
	vcvtbiasph2bf8 xmm15, ymm14, ymm9
	vcvtbiasph2bf8 ymm9, zmm14, zmm11
	vcvtbiasph2bf8 xmm25, xmm16, xmm28
	vcvtbiasph2bf8 xmm18, ymm19, ymm17
	vcvtbiasph2bf8 ymm24, zmm16, zmm23
%endif
