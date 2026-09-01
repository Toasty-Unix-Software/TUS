	vrsqrt14ps xmm2, oword [0x188]
	vrsqrt14ps xmm2, oword [0xb04]
	vrsqrt14ps ymm2, ymm2
	vrsqrt14ps ymm5, ymm7
	vrsqrt14ps zmm0, zmm2
	vrsqrt14ps zmm3, zword [0x1b5]

%ifdef ERROR
	vrsqrt14ps xmm11, xmm13
	vrsqrt14ps ymm8, ymm14
	vrsqrt14ps zmm14, zmm8
	vrsqrt14ps xmm19, xmm25
	vrsqrt14ps ymm20, ymm27
	vrsqrt14ps zmm20, zmm31
%endif
