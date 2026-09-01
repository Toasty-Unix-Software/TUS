	vpandnd xmm5, xmm3, xmm2
	vpandnd xmm5, xmm2
	vpandnd xmm7, xmm4, oword [0xeb6]
	vpandnd ymm5, ymm3, ymm6
	vpandnd ymm5, ymm6
	vpandnd ymm5, ymm2, ymm7
	vpandnd zmm4, zmm0, zword [0x7bc]
	vpandnd zmm4, zword [0x7bc]
	vpandnd zmm0, zmm1, zword [0x292]

%ifdef ERROR
	vpandnd xmm14, xmm11, xmm13
	vpandnd ymm14, ymm11, ymm10
	vpandnd zmm9, zmm10, zmm14
	vpandnd xmm24, xmm17, xmm31
	vpandnd ymm19, ymm28, ymm21
	vpandnd zmm17, zmm18, zmm16
%endif
