	vcvtph2iubs xmm3, oword [0xe53]
	vcvtph2iubs xmm7, xmm7
	vcvtph2iubs ymm1, ymm2
	vcvtph2iubs ymm7, yword [0xa08]
	vcvtph2iubs zmm3, zword [0xdf3]
	vcvtph2iubs zmm4, zword [0x83a]

%ifdef ERROR
	vcvtph2iubs xmm10, xmm15
	vcvtph2iubs ymm15, ymm8
	vcvtph2iubs zmm11, zmm12
	vcvtph2iubs xmm27, xmm30
	vcvtph2iubs ymm25, ymm20
	vcvtph2iubs zmm27, zmm25
%endif
