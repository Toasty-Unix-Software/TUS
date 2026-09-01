default rel
	vpcmpleuq k1, xmm4, oword [0x430]
	vpcmpleuq k7, xmm6, xmm1
	vpcmpleuq k2, ymm0, ymm1
	vpcmpleuq k5, ymm7, ymm4
	vpcmpleuq k3, zmm2, zmm0
	vpcmpleuq k6, zmm6, zword [0xe44]
	vpcmpleuq k7, xmm14, xmm10
	vpcmpleuq k1, ymm11, ymm12
	vpcmpleuq k4, zmm11, zmm15
	vpcmpleuq k5, xmm29, xmm24
	vpcmpleuq k4, ymm22, ymm26
	vpcmpleuq k6, zmm23, zmm28
	vpcmpleuq k5{k6}, xmm0, oword [0x8f4]
	vpcmpleuq k7{k6}, ymm0, ymm3
	vpcmpleuq k5{k4}, zmm7, zmm4
	vpcmpleuq k4, xmm3, oword [eax+1]
	vpcmpleuq k7, xmm3, oword [eax+64]
	vpcmpleuq k1, ymm5, yword [eax+1]
	vpcmpleuq k4, ymm3, yword [eax+64]
	vpcmpleuq k1, zmm5, zword [eax+1]
	vpcmpleuq k3, zmm7, zword [eax+64]
	vpcmpleuq k6, xmm5, [0x334]
	vpcmpleuq k7, ymm5, [0xa16]
	vpcmpleuq k2, zmm1, [0x9d7]
