default rel
	vpcmov xmm2, xmm1, xmm4, xmm4
	vpcmov xmm2, xmm4, xmm4
	vpcmov xmm7, xmm2, oword [0xcf0], xmm5
	vpcmov ymm4, ymm3, yword [0x3a4], ymm1
	vpcmov ymm4, yword [0x3a4], ymm1
	vpcmov ymm2, ymm5, ymm4, ymm7
	vpcmov xmm1, xmm6, xmm3, xmm0
	vpcmov xmm1, xmm3, xmm0
	vpcmov xmm0, xmm3, xmm5, oword [0x8e4]
	vpcmov ymm1, ymm2, ymm1, yword [0xe95]
	vpcmov ymm1, ymm1, yword [0xe95]
	vpcmov ymm1, ymm4, ymm0, ymm2
	vpcmov xmm13, xmm15, xmm11, xmm8
	vpcmov ymm11, ymm8, ymm13, ymm14
	vpcmov xmm15, xmm8, xmm8, xmm10
	vpcmov ymm10, ymm15, ymm15, ymm10
	vpcmov xmm6, xmm0, oword [eax+1], xmm6
	vpcmov xmm3, xmm2, oword [eax+64], xmm0
	vpcmov ymm6, ymm5, yword [eax+1], ymm3
	vpcmov ymm4, ymm7, yword [eax+64], ymm1
	vpcmov xmm1, xmm6, xmm1, oword [eax+1]
	vpcmov xmm4, xmm7, xmm6, oword [eax+64]
	vpcmov ymm5, ymm7, ymm5, yword [eax+1]
	vpcmov ymm4, ymm1, ymm1, yword [eax+64]
	vpcmov xmm0, xmm1, [0x8ae], xmm5
	vpcmov ymm4, ymm5, [0xb78], ymm7
	vpcmov xmm4, xmm3, xmm3, [0x889]
	vpcmov ymm4, ymm7, ymm3, [0xc2d]
