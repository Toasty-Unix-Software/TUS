default rel
	vpcmpneqb k4, xmm5, xmm6
	vpcmpneqb k5, xmm6, xmm5
	vpcmpneqb k4, ymm2, yword [0x81a]
	vpcmpneqb k2, ymm1, yword [0x809]
	vpcmpneqb k3, zmm5, zmm0
	vpcmpneqb k1, zmm3, zword [0x95c]
	vpcmpneqb k6, xmm9, xmm8
	vpcmpneqb k3, ymm14, ymm13
	vpcmpneqb k4, zmm10, zmm12
	vpcmpneqb k2, xmm23, xmm24
	vpcmpneqb k2, ymm28, ymm19
	vpcmpneqb k3, zmm31, zmm31
	vpcmpneqb k3{k7}, xmm4, xmm1
	vpcmpneqb k4{k2}, ymm5, ymm4
	vpcmpneqb k4{k6}, zmm1, zword [0x346]
	vpcmpneqb k2, xmm4, oword [eax+1]
	vpcmpneqb k7, xmm7, oword [eax+64]
	vpcmpneqb k2, ymm1, yword [eax+1]
	vpcmpneqb k3, ymm1, yword [eax+64]
	vpcmpneqb k2, zmm7, zword [eax+1]
	vpcmpneqb k6, zmm6, zword [eax+64]
	vpcmpneqb k4, xmm6, [0xb20]
	vpcmpneqb k2, ymm3, [0xd54]
	vpcmpneqb k1, zmm4, [0x203]
