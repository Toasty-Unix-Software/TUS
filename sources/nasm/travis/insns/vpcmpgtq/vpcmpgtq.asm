default rel
	vpcmpgtq xmm0, xmm3, xmm2
	vpcmpgtq xmm0, xmm2
	vpcmpgtq xmm6, xmm0, xmm6
	vpcmpgtq ymm2, ymm4, ymm5
	vpcmpgtq ymm2, ymm5
	vpcmpgtq ymm0, ymm7, ymm2
	vpcmpgtq k1, xmm1, oword [0x68e]
	vpcmpgtq k7, xmm3, oword [0xe6b]
	vpcmpgtq k2, ymm3, yword [0x7ba]
	vpcmpgtq k7, ymm1, ymm6
	vpcmpgtq xmm8, xmm13, xmm13
	vpcmpgtq ymm13, ymm10, ymm11
	vpcmpgtq k3, xmm8, xmm9
	vpcmpgtq k2, ymm11, ymm14
	vpcmpgtq k6{k4}, xmm4, xmm1
	vpcmpgtq k4{k2}, ymm2, yword [0x4d1]
	vpcmpgtq k7{k2}, zmm0, zmm0
	vpcmpgtq xmm7, xmm2, oword [eax+1]
	vpcmpgtq xmm1, xmm1, oword [eax+64]
	vpcmpgtq ymm7, ymm3, yword [eax+1]
	vpcmpgtq ymm3, ymm6, yword [eax+64]
	vpcmpgtq k3, xmm1, oword [eax+1]
	vpcmpgtq k3, xmm7, oword [eax+64]
	vpcmpgtq k2, ymm6, yword [eax+1]
	vpcmpgtq k5, ymm6, yword [eax+64]
	vpcmpgtq k3, zmm2, zword [eax+1]
	vpcmpgtq k1, zmm0, zword [eax+64]
	vpcmpgtq xmm4, xmm0, [0xf4b]
	vpcmpgtq ymm1, ymm5, [0x6d9]
	vpcmpgtq k6, xmm1, [0xc82]
	vpcmpgtq k2, ymm6, [0x834]
	vpcmpgtq k1, zmm0, [0xfc1]
