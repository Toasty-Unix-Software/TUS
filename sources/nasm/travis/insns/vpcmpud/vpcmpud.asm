default rel
	vpcmpud k6, xmm7, xmm2, 0xda
	vpcmpud k2, xmm3, xmm0, 0xc1
	vpcmpud k4, ymm6, ymm0, 0x33
	vpcmpud k5, ymm3, ymm6, 0x96
	vpcmpud k4, zmm7, zword [0xbd8], 0x4
	vpcmpud k3, zmm4, zmm6, 0x67
	vpcmpud k3, xmm10, xmm12, 0x6
	vpcmpud k3, ymm13, ymm13, 0xda
	vpcmpud k4, zmm10, zmm12, 0x7d
	vpcmpud k4, xmm20, xmm25, 0x2
	vpcmpud k2, ymm28, ymm20, 0xde
	vpcmpud k6, zmm21, zmm30, 0x99
	vpcmpud k6{k2}, xmm7, xmm2, 0xc9
	vpcmpud k7{k4}, ymm5, ymm1, 0x18
	vpcmpud k1{k5}, zmm1, zmm2, 0xee
	vpcmpud k1, xmm0, oword [eax+1], 0xb1
	vpcmpud k7, xmm6, oword [eax+64], 0xf8
	vpcmpud k3, ymm0, yword [eax+1], 0xa6
	vpcmpud k7, ymm4, yword [eax+64], 0x7b
	vpcmpud k7, zmm3, zword [eax+1], 0x66
	vpcmpud k2, zmm2, zword [eax+64], 0x5
	vpcmpud k6, xmm1, [0xdfd], 0xca
	vpcmpud k6, ymm3, [0x726], 0x30
	vpcmpud k5, zmm2, [0xf6e], 0x7f
