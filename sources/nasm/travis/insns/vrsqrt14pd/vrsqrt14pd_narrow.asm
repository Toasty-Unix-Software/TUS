	vrsqrt14pd xmm2, xmm6
	vrsqrt14pd xmm2, xmm4
	vrsqrt14pd ymm0, ymm4
	vrsqrt14pd ymm7, ymm3
	vrsqrt14pd zmm1, zmm3
	vrsqrt14pd zmm5, zmm1

%ifdef ERROR
	vrsqrt14pd xmm15, xmm11
	vrsqrt14pd ymm11, ymm8
	vrsqrt14pd zmm14, zmm11
	vrsqrt14pd xmm29, xmm17
	vrsqrt14pd ymm23, ymm21
	vrsqrt14pd zmm19, zmm20
%endif
