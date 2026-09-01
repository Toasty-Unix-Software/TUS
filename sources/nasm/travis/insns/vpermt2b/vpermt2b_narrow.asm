	vpermt2b xmm5, xmm2, xmm5
	vpermt2b xmm1, xmm3, xmm6
	vpermt2b ymm1, ymm6, ymm1
	vpermt2b ymm4, ymm3, ymm4
	vpermt2b zmm2, zmm7, zmm7
	vpermt2b zmm0, zmm7, zmm0

%ifdef ERROR
	vpermt2b xmm10, xmm13, xmm8
	vpermt2b ymm10, ymm8, ymm12
	vpermt2b zmm12, zmm9, zmm14
	vpermt2b xmm27, xmm18, xmm17
	vpermt2b ymm18, ymm28, ymm20
	vpermt2b zmm22, zmm26, zmm20
%endif
