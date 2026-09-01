	vpermi2pd xmm1, xmm2, oword [0x857]
	vpermi2pd xmm2, xmm2, xmm7
	vpermi2pd ymm1, ymm3, ymm7
	vpermi2pd ymm4, ymm1, ymm4
	vpermi2pd zmm0, zmm5, zword [0x1e2]
	vpermi2pd zmm5, zmm6, zmm2

%ifdef ERROR
	vpermi2pd xmm15, xmm14, xmm10
	vpermi2pd ymm10, ymm9, ymm9
	vpermi2pd zmm8, zmm10, zmm11
	vpermi2pd xmm25, xmm24, xmm21
	vpermi2pd ymm29, ymm22, ymm17
	vpermi2pd zmm26, zmm18, zmm17
%endif
