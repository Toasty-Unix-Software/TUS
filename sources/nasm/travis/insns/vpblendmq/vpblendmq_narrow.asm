	vpblendmq xmm6, xmm7, oword [0x7bd]
	vpblendmq xmm3, xmm0, xmm3
	vpblendmq ymm2, ymm5, yword [0xcc3]
	vpblendmq ymm3, ymm4, yword [0x3f3]
	vpblendmq zmm4, zmm6, zword [0x23b]
	vpblendmq zmm1, zmm4, zmm2

%ifdef ERROR
	vpblendmq xmm9, xmm10, xmm8
	vpblendmq ymm11, ymm13, ymm8
	vpblendmq zmm13, zmm12, zmm14
	vpblendmq xmm30, xmm27, xmm28
	vpblendmq ymm20, ymm22, ymm20
	vpblendmq zmm24, zmm23, zmm27
%endif
