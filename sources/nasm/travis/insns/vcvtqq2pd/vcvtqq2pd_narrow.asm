	vcvtqq2pd xmm5, xmm6
	vcvtqq2pd xmm4, xmm2
	vcvtqq2pd ymm6, ymm0
	vcvtqq2pd ymm6, ymm3
	vcvtqq2pd zmm1, zword [0x37f]
	vcvtqq2pd zmm2, zmm4

%ifdef ERROR
	vcvtqq2pd xmm15, xmm12
	vcvtqq2pd ymm15, ymm10
	vcvtqq2pd zmm10, zmm8
	vcvtqq2pd xmm21, xmm16
	vcvtqq2pd ymm25, ymm28
	vcvtqq2pd zmm17, zmm29
%endif
