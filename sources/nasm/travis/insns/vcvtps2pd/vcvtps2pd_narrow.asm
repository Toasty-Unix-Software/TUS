	vcvtps2pd xmm4, xmm0
	vcvtps2pd xmm6, xmm5
	vcvtps2pd ymm0, xmm4
	vcvtps2pd ymm0, oword [0xcc5]
	vcvtps2pd xmm1, xmm4
	vcvtps2pd xmm4, xmm0
	vcvtps2pd ymm5, xmm2
	vcvtps2pd ymm3, oword [0xdd3]

%ifdef ERROR
	vcvtps2pd xmm13, xmm11
	vcvtps2pd ymm13, xmm12
	vcvtps2pd xmm8, xmm15
	vcvtps2pd ymm11, xmm8
	vcvtps2pd xmm31, xmm18
	vcvtps2pd ymm23, xmm24
	vcvtps2pd xmm27, xmm22
	vcvtps2pd ymm25, xmm25
%endif
