	vcvtudq2pd xmm3, xmm2
	vcvtudq2pd xmm7, qword [0x9b5]
	vcvtudq2pd ymm1, xmm5
	vcvtudq2pd ymm5, xmm5
	vcvtudq2pd zmm4, ymm0
	vcvtudq2pd zmm1, ymm4

%ifdef ERROR
	vcvtudq2pd xmm8, xmm10
	vcvtudq2pd ymm11, xmm15
	vcvtudq2pd zmm8, ymm14
	vcvtudq2pd xmm28, xmm20
	vcvtudq2pd ymm25, xmm28
	vcvtudq2pd zmm23, ymm26
%endif
