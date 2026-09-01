	vpblendmw xmm0, xmm6, xmm0
	vpblendmw xmm1, xmm0, xmm7
	vpblendmw ymm3, ymm2, yword [0xe21]
	vpblendmw ymm6, ymm6, yword [0x257]
	vpblendmw zmm1, zmm7, zword [0xa16]
	vpblendmw zmm7, zmm5, zmm1

%ifdef ERROR
	vpblendmw xmm10, xmm8, xmm15
	vpblendmw ymm13, ymm15, ymm12
	vpblendmw zmm9, zmm14, zmm12
	vpblendmw xmm31, xmm24, xmm21
	vpblendmw ymm24, ymm16, ymm26
	vpblendmw zmm20, zmm30, zmm22
%endif
