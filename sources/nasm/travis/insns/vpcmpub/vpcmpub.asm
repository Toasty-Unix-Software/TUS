default rel
	vpcmpub k3, xmm5, xmm6, 0x28
	vpcmpub k6, xmm0, oword [0xaab], 0xd0
	vpcmpub k4, ymm4, ymm1, 0x5b
	vpcmpub k3, ymm4, ymm6, 0xc0
	vpcmpub k3, zmm2, zmm1, 0x40
	vpcmpub k4, zmm2, zmm0, 0x75
	vpcmpub k3, xmm8, xmm14, 0x2a
	vpcmpub k5, ymm12, ymm15, 0x41
	vpcmpub k3, zmm9, zmm13, 0xb3
	vpcmpub k3, xmm31, xmm18, 0xd2
	vpcmpub k5, ymm22, ymm23, 0x57
	vpcmpub k6, zmm19, zmm25, 0xe
	vpcmpub k4{k1}, xmm2, oword [0x51d], 0x3e
	vpcmpub k7{k3}, ymm6, yword [0x526], 0x44
	vpcmpub k7{k5}, zmm0, zmm3, 0x4
	vpcmpub k6, xmm2, oword [eax+1], 0xa8
	vpcmpub k3, xmm5, oword [eax+64], 0xa0
	vpcmpub k4, ymm6, yword [eax+1], 0x73
	vpcmpub k7, ymm5, yword [eax+64], 0x96
	vpcmpub k1, zmm0, zword [eax+1], 0xee
	vpcmpub k5, zmm5, zword [eax+64], 0x33
	vpcmpub k7, xmm7, [0xfd2], 0xb3
	vpcmpub k2, ymm1, [0x49f], 0x4f
	vpcmpub k4, zmm2, [0x1d7], 0xc9
