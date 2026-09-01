	vpermt2d xmm7, xmm2, xmm6
	vpermt2d xmm4, xmm4, xmm7
	vpermt2d ymm6, ymm7, ymm5
	vpermt2d ymm4, ymm6, ymm7
	vpermt2d zmm2, zmm0, zword [0x10e]
	vpermt2d zmm5, zmm3, zmm7

%ifdef ERROR
	vpermt2d xmm8, xmm10, xmm12
	vpermt2d ymm8, ymm13, ymm8
	vpermt2d zmm14, zmm13, zmm12
	vpermt2d xmm28, xmm25, xmm24
	vpermt2d ymm17, ymm23, ymm16
	vpermt2d zmm18, zmm31, zmm28
%endif
