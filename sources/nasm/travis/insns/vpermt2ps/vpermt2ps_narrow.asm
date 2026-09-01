	vpermt2ps xmm2, xmm5, xmm6
	vpermt2ps xmm6, xmm6, xmm2
	vpermt2ps ymm1, ymm3, ymm0
	vpermt2ps ymm2, ymm0, ymm1
	vpermt2ps zmm5, zmm3, zword [0x165]
	vpermt2ps zmm7, zmm6, zmm4

%ifdef ERROR
	vpermt2ps xmm10, xmm11, xmm12
	vpermt2ps ymm11, ymm9, ymm12
	vpermt2ps zmm11, zmm14, zmm10
	vpermt2ps xmm22, xmm27, xmm30
	vpermt2ps ymm28, ymm28, ymm22
	vpermt2ps zmm31, zmm26, zmm25
%endif
