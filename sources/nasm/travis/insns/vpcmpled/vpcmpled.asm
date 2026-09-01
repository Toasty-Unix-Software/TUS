default rel
	vpcmpled k2, xmm1, xmm6
	vpcmpled k1, xmm7, oword [0x664]
	vpcmpled k7, ymm3, yword [0x62a]
	vpcmpled k7, ymm1, ymm3
	vpcmpled k3, zmm0, zword [0x9a2]
	vpcmpled k4, zmm5, zword [0xfa1]
	vpcmpled k3, xmm9, xmm12
	vpcmpled k3, ymm10, ymm15
	vpcmpled k6, zmm15, zmm12
	vpcmpled k3, xmm23, xmm31
	vpcmpled k5, ymm20, ymm21
	vpcmpled k2, zmm22, zmm17
	vpcmpled k2{k7}, xmm1, xmm1
	vpcmpled k3{k2}, ymm2, ymm3
	vpcmpled k6{k5}, zmm7, zmm6
	vpcmpled k7, xmm4, oword [eax+1]
	vpcmpled k3, xmm6, oword [eax+64]
	vpcmpled k2, ymm7, yword [eax+1]
	vpcmpled k4, ymm7, yword [eax+64]
	vpcmpled k7, zmm5, zword [eax+1]
	vpcmpled k5, zmm6, zword [eax+64]
	vpcmpled k5, xmm3, [0xe7a]
	vpcmpled k2, ymm4, [0xbde]
	vpcmpled k2, zmm1, [0xc49]
