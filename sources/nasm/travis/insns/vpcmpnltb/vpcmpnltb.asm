default rel
	vpcmpnltb k7, xmm7, xmm3
	vpcmpnltb k6, xmm3, oword [0x48a]
	vpcmpnltb k3, ymm6, yword [0xdf9]
	vpcmpnltb k2, ymm1, ymm4
	vpcmpnltb k2, zmm3, zmm6
	vpcmpnltb k6, zmm7, zword [0xf5f]
	vpcmpnltb k7, xmm14, xmm11
	vpcmpnltb k5, ymm8, ymm12
	vpcmpnltb k4, zmm12, zmm11
	vpcmpnltb k5, xmm17, xmm16
	vpcmpnltb k3, ymm21, ymm30
	vpcmpnltb k5, zmm23, zmm31
	vpcmpnltb k5{k4}, xmm5, oword [0x73d]
	vpcmpnltb k1{k3}, ymm7, yword [0xb62]
	vpcmpnltb k3{k6}, zmm5, zmm7
	vpcmpnltb k3, xmm6, oword [eax+1]
	vpcmpnltb k4, xmm0, oword [eax+64]
	vpcmpnltb k3, ymm5, yword [eax+1]
	vpcmpnltb k1, ymm0, yword [eax+64]
	vpcmpnltb k2, zmm0, zword [eax+1]
	vpcmpnltb k2, zmm4, zword [eax+64]
	vpcmpnltb k5, xmm3, [0xbc7]
	vpcmpnltb k2, ymm6, [0xc76]
	vpcmpnltb k3, zmm2, [0x257]
