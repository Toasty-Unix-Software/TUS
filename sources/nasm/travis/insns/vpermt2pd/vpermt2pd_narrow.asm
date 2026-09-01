	vpermt2pd xmm0, xmm2, oword [0xc75]
	vpermt2pd xmm2, xmm0, xmm0
	vpermt2pd ymm3, ymm5, ymm2
	vpermt2pd ymm5, ymm5, ymm1
	vpermt2pd zmm7, zmm0, zmm1
	vpermt2pd zmm6, zmm4, zmm3

%ifdef ERROR
	vpermt2pd xmm15, xmm10, xmm10
	vpermt2pd ymm11, ymm13, ymm13
	vpermt2pd zmm8, zmm15, zmm12
	vpermt2pd xmm28, xmm18, xmm27
	vpermt2pd ymm16, ymm30, ymm26
	vpermt2pd zmm23, zmm19, zmm16
%endif
