	vcvtuqq2pd xmm7, oword [0xfeb]
	vcvtuqq2pd xmm0, xmm6
	vcvtuqq2pd ymm6, ymm1
	vcvtuqq2pd ymm3, ymm4
	vcvtuqq2pd zmm5, zword [0xcc5]
	vcvtuqq2pd zmm4, zmm6

%ifdef ERROR
	vcvtuqq2pd xmm8, xmm12
	vcvtuqq2pd ymm9, ymm12
	vcvtuqq2pd zmm15, zmm14
	vcvtuqq2pd xmm23, xmm19
	vcvtuqq2pd ymm19, ymm21
	vcvtuqq2pd zmm27, zmm18
%endif
