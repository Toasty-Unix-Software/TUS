	vpxorq xmm2, xmm0, xmm7
	vpxorq xmm2, xmm7
	vpxorq xmm1, xmm2, xmm7
	vpxorq ymm7, ymm2, ymm4
	vpxorq ymm7, ymm4
	vpxorq ymm6, ymm3, yword [0x818]
	vpxorq zmm2, zmm5, zword [0x11a]
	vpxorq zmm2, zword [0x11a]
	vpxorq zmm0, zmm5, zmm5

%ifdef ERROR
	vpxorq xmm15, xmm10, xmm12
	vpxorq ymm15, ymm13, ymm11
	vpxorq zmm11, zmm11, zmm15
	vpxorq xmm19, xmm30, xmm16
	vpxorq ymm27, ymm30, ymm23
	vpxorq zmm21, zmm30, zmm31
%endif
