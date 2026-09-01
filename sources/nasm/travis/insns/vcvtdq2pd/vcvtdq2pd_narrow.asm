	vcvtdq2pd xmm4, xmm0
	vcvtdq2pd xmm5, qword [0x2d3]
	vcvtdq2pd ymm7, oword [0x246]
	vcvtdq2pd ymm3, xmm4
	vcvtdq2pd xmm1, xmm3
	vcvtdq2pd xmm6, xmm6
	vcvtdq2pd ymm4, xmm6
	vcvtdq2pd ymm1, xmm7

%ifdef ERROR
	vcvtdq2pd xmm15, xmm8
	vcvtdq2pd ymm8, xmm10
	vcvtdq2pd xmm14, xmm8
	vcvtdq2pd ymm11, xmm10
	vcvtdq2pd xmm22, xmm16
	vcvtdq2pd ymm27, xmm21
	vcvtdq2pd xmm18, xmm28
	vcvtdq2pd ymm23, xmm25
%endif
