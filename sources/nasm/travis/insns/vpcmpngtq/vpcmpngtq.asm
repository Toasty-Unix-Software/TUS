default rel
	vpcmpngtq k2, xmm4, xmm0
	vpcmpngtq k4, xmm0, xmm6
	vpcmpngtq k4, ymm7, ymm2
	vpcmpngtq k6, ymm4, yword [0x9ed]
	vpcmpngtq k3, zmm6, zword [0xaa2]
	vpcmpngtq k4, zmm5, zmm2
	vpcmpngtq k3, xmm11, xmm14
	vpcmpngtq k2, ymm10, ymm12
	vpcmpngtq k7, zmm10, zmm15
	vpcmpngtq k1, xmm16, xmm19
	vpcmpngtq k7, ymm28, ymm16
	vpcmpngtq k1, zmm23, zmm16
	vpcmpngtq k6{k4}, xmm6, xmm3
	vpcmpngtq k1{k2}, ymm4, yword [0xd44]
	vpcmpngtq k4{k3}, zmm4, zmm7
	vpcmpngtq k5, xmm1, oword [eax+1]
	vpcmpngtq k1, xmm0, oword [eax+64]
	vpcmpngtq k6, ymm4, yword [eax+1]
	vpcmpngtq k3, ymm0, yword [eax+64]
	vpcmpngtq k5, zmm0, zword [eax+1]
	vpcmpngtq k3, zmm2, zword [eax+64]
	vpcmpngtq k6, xmm3, [0x457]
	vpcmpngtq k1, ymm6, [0x423]
	vpcmpngtq k2, zmm2, [0x97d]
