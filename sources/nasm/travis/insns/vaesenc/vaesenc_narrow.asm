	vaesenc xmm6, xmm4, xmm2
	vaesenc xmm6, xmm2
	vaesenc xmm3, xmm7, xmm1
	vaesenc ymm1, ymm4, ymm0
	vaesenc ymm1, ymm0
	vaesenc ymm5, ymm0, ymm7
	vaesenc zmm0, zmm4, zmm2
	vaesenc zmm0, zmm2
	vaesenc zmm6, zmm1, zmm0

%ifdef ERROR
	vaesenc xmm12, xmm11, xmm15
	vaesenc ymm9, ymm8, ymm10
	vaesenc zmm14, zmm15, zmm11
	vaesenc xmm24, xmm17, xmm16
	vaesenc ymm19, ymm19, ymm27
	vaesenc zmm27, zmm31, zmm22
%endif
