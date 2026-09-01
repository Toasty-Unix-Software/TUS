	vcvtph2uw xmm7, xmm4
	vcvtph2uw xmm5, xmm4
	vcvtph2uw ymm0, ymm0
	vcvtph2uw ymm4, ymm3
	vcvtph2uw zmm4, zword [0xc35]
	vcvtph2uw zmm7, zmm4

%ifdef ERROR
	vcvtph2uw xmm14, xmm12
	vcvtph2uw ymm15, ymm10
	vcvtph2uw zmm10, zmm13
	vcvtph2uw xmm27, xmm18
	vcvtph2uw ymm16, ymm25
	vcvtph2uw zmm23, zmm20
%endif
