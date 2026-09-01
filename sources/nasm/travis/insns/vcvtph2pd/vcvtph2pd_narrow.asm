	vcvtph2pd xmm5, dword [0x3c4]
	vcvtph2pd xmm4, xmm4
	vcvtph2pd ymm6, xmm2
	vcvtph2pd ymm2, xmm7
	vcvtph2pd zmm6, xmm2
	vcvtph2pd zmm5, xmm3

%ifdef ERROR
	vcvtph2pd xmm10, xmm12
	vcvtph2pd ymm10, xmm10
	vcvtph2pd zmm14, xmm15
	vcvtph2pd xmm18, xmm25
	vcvtph2pd ymm31, xmm16
	vcvtph2pd zmm26, xmm16
%endif
