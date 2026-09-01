default rel
	vpblendmq xmm6, xmm7, oword [0x7bd]
	vpblendmq xmm3, xmm0, xmm3
	vpblendmq ymm2, ymm5, yword [0xcc3]
	vpblendmq ymm3, ymm4, yword [0x3f3]
	vpblendmq zmm4, zmm6, zword [0x23b]
	vpblendmq zmm1, zmm4, zmm2
	vpblendmq xmm9, xmm10, xmm8
	vpblendmq ymm11, ymm13, ymm8
	vpblendmq zmm13, zmm12, zmm14
	vpblendmq xmm30, xmm27, xmm28
	vpblendmq ymm20, ymm22, ymm20
	vpblendmq zmm24, zmm23, zmm27
	vpblendmq xmm1{k6}, xmm3, xmm1
	vpblendmq ymm6{k2}, ymm1, ymm1
	vpblendmq zmm0{k7}, zmm3, zword [0xe25]
	vpblendmq xmm4{k4}{z}, xmm2, oword [0xbda]
	vpblendmq ymm7{k5}{z}, ymm6, yword [0xe68]
	vpblendmq zmm6{k7}{z}, zmm5, zmm2
	vpblendmq xmm2, xmm0, oword [eax+1]
	vpblendmq xmm6, xmm4, oword [eax+64]
	vpblendmq ymm6, ymm3, yword [eax+1]
	vpblendmq ymm7, ymm1, yword [eax+64]
	vpblendmq zmm5, zmm6, zword [eax+1]
	vpblendmq zmm6, zmm5, zword [eax+64]
	vpblendmq xmm0, xmm2, [0x21f]
	vpblendmq ymm6, ymm7, [0x870]
	vpblendmq zmm7, zmm2, [0xe32]
