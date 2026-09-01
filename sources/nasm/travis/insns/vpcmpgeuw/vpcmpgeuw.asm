default rel
	vpcmpgeuw k1, xmm2, oword [0xf6b]
	vpcmpgeuw k5, xmm6, xmm1
	vpcmpgeuw k2, ymm7, yword [0xa78]
	vpcmpgeuw k7, ymm1, ymm0
	vpcmpgeuw k6, zmm4, zmm3
	vpcmpgeuw k3, zmm4, zmm1
	vpcmpgeuw k7, xmm11, xmm10
	vpcmpgeuw k6, ymm8, ymm10
	vpcmpgeuw k6, zmm11, zmm12
	vpcmpgeuw k6, xmm30, xmm22
	vpcmpgeuw k1, ymm27, ymm31
	vpcmpgeuw k7, zmm23, zmm19
	vpcmpgeuw k4{k6}, xmm1, xmm0
	vpcmpgeuw k5{k5}, ymm2, ymm5
	vpcmpgeuw k2{k1}, zmm0, zword [0x37a]
	vpcmpgeuw k5, xmm7, oword [eax+1]
	vpcmpgeuw k3, xmm1, oword [eax+64]
	vpcmpgeuw k7, ymm0, yword [eax+1]
	vpcmpgeuw k3, ymm0, yword [eax+64]
	vpcmpgeuw k4, zmm4, zword [eax+1]
	vpcmpgeuw k1, zmm4, zword [eax+64]
	vpcmpgeuw k3, xmm0, [0xb7e]
	vpcmpgeuw k5, ymm5, [0xf00]
	vpcmpgeuw k4, zmm7, [0x316]
