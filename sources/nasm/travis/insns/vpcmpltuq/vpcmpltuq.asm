default rel
	vpcmpltuq k5, xmm0, xmm0
	vpcmpltuq k2, xmm0, xmm4
	vpcmpltuq k7, ymm2, ymm0
	vpcmpltuq k7, ymm6, ymm3
	vpcmpltuq k5, zmm6, zmm2
	vpcmpltuq k6, zmm6, zword [0x7bf]
	vpcmpltuq k4, xmm13, xmm12
	vpcmpltuq k6, ymm15, ymm12
	vpcmpltuq k7, zmm14, zmm11
	vpcmpltuq k1, xmm20, xmm29
	vpcmpltuq k1, ymm21, ymm22
	vpcmpltuq k3, zmm17, zmm26
	vpcmpltuq k6{k2}, xmm5, oword [0x3f6]
	vpcmpltuq k7{k6}, ymm7, yword [0x375]
	vpcmpltuq k7{k6}, zmm0, zmm4
	vpcmpltuq k5, xmm4, oword [eax+1]
	vpcmpltuq k5, xmm4, oword [eax+64]
	vpcmpltuq k3, ymm2, yword [eax+1]
	vpcmpltuq k7, ymm6, yword [eax+64]
	vpcmpltuq k2, zmm1, zword [eax+1]
	vpcmpltuq k2, zmm0, zword [eax+64]
	vpcmpltuq k5, xmm1, [0x6c0]
	vpcmpltuq k1, ymm1, [0x715]
	vpcmpltuq k5, zmm3, [0xfc5]
