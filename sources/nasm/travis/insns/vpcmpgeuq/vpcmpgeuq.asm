default rel
	vpcmpgeuq k2, xmm2, oword [0x75c]
	vpcmpgeuq k6, xmm2, oword [0x81e]
	vpcmpgeuq k4, ymm3, yword [0xbf1]
	vpcmpgeuq k7, ymm2, yword [0x11e]
	vpcmpgeuq k7, zmm2, zmm3
	vpcmpgeuq k6, zmm0, zmm3
	vpcmpgeuq k5, xmm13, xmm14
	vpcmpgeuq k1, ymm12, ymm11
	vpcmpgeuq k6, zmm15, zmm14
	vpcmpgeuq k3, xmm18, xmm16
	vpcmpgeuq k1, ymm23, ymm27
	vpcmpgeuq k3, zmm31, zmm29
	vpcmpgeuq k6{k2}, xmm1, oword [0xca6]
	vpcmpgeuq k6{k3}, ymm1, yword [0xcce]
	vpcmpgeuq k6{k2}, zmm6, zword [0x360]
	vpcmpgeuq k1, xmm2, oword [eax+1]
	vpcmpgeuq k2, xmm1, oword [eax+64]
	vpcmpgeuq k2, ymm4, yword [eax+1]
	vpcmpgeuq k4, ymm6, yword [eax+64]
	vpcmpgeuq k6, zmm6, zword [eax+1]
	vpcmpgeuq k6, zmm4, zword [eax+64]
	vpcmpgeuq k4, xmm6, [0x3eb]
	vpcmpgeuq k5, ymm7, [0xd00]
	vpcmpgeuq k1, zmm4, [0x2ec]
