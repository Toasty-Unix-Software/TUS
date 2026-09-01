	vpermi2b xmm6, xmm3, oword [0x29f]
	vpermi2b xmm6, xmm1, xmm7
	vpermi2b ymm7, ymm2, ymm0
	vpermi2b ymm6, ymm3, ymm6
	vpermi2b zmm6, zmm3, zmm5
	vpermi2b zmm1, zmm4, zword [0xe3a]

%ifdef ERROR
	vpermi2b xmm14, xmm14, xmm10
	vpermi2b ymm15, ymm11, ymm9
	vpermi2b zmm8, zmm12, zmm10
	vpermi2b xmm23, xmm17, xmm30
	vpermi2b ymm22, ymm23, ymm23
	vpermi2b zmm31, zmm26, zmm30
%endif
