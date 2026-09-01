	vpermt2q xmm7, xmm1, xmm5
	vpermt2q xmm0, xmm4, xmm7
	vpermt2q ymm2, ymm3, ymm6
	vpermt2q ymm1, ymm1, yword [0x9e2]
	vpermt2q zmm4, zmm7, zword [0x84d]
	vpermt2q zmm5, zmm7, zmm4

%ifdef ERROR
	vpermt2q xmm8, xmm11, xmm8
	vpermt2q ymm12, ymm15, ymm11
	vpermt2q zmm10, zmm12, zmm11
	vpermt2q xmm25, xmm21, xmm17
	vpermt2q ymm21, ymm18, ymm24
	vpermt2q zmm23, zmm22, zmm21
%endif
