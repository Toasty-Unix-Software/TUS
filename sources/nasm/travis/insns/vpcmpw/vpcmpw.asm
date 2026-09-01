default rel
	vpcmpw k4, xmm6, xmm7, 0xa2
	vpcmpw k7, xmm4, oword [0x3ef], 0x22
	vpcmpw k4, ymm3, ymm7, 0xab
	vpcmpw k7, ymm3, ymm4, 0x96
	vpcmpw k4, zmm5, zmm1, 0xc1
	vpcmpw k1, zmm1, zword [0x693], 0xc0
	vpcmpw k4, xmm9, xmm13, 0x74
	vpcmpw k5, ymm10, ymm12, 0x56
	vpcmpw k3, zmm13, zmm13, 0x89
	vpcmpw k2, xmm28, xmm23, 0xe8
	vpcmpw k5, ymm18, ymm22, 0xe3
	vpcmpw k2, zmm29, zmm17, 0x29
	vpcmpw k6{k2}, xmm3, xmm5, 0xd4
	vpcmpw k4{k1}, ymm3, yword [0xc02], 0x18
	vpcmpw k5{k1}, zmm2, zword [0xdfe], 0x2b
	vpcmpw k2, xmm2, oword [eax+1], 0xb5
	vpcmpw k1, xmm2, oword [eax+64], 0x92
	vpcmpw k6, ymm2, yword [eax+1], 0x24
	vpcmpw k2, ymm1, yword [eax+64], 0xc2
	vpcmpw k3, zmm0, zword [eax+1], 0x57
	vpcmpw k2, zmm3, zword [eax+64], 0xef
	vpcmpw k1, xmm3, [0x7e1], 0xb9
	vpcmpw k3, ymm6, [0xaae], 0xb2
	vpcmpw k2, zmm2, [0x83c], 0x89
