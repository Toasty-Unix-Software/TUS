default rel
	vpcmpnleb k4, xmm3, oword [0xd74]
	vpcmpnleb k3, xmm6, xmm7
	vpcmpnleb k5, ymm5, ymm6
	vpcmpnleb k1, ymm2, ymm2
	vpcmpnleb k7, zmm3, zmm6
	vpcmpnleb k6, zmm1, zmm3
	vpcmpnleb k3, xmm9, xmm9
	vpcmpnleb k1, ymm11, ymm12
	vpcmpnleb k6, zmm9, zmm9
	vpcmpnleb k6, xmm31, xmm25
	vpcmpnleb k6, ymm19, ymm20
	vpcmpnleb k7, zmm22, zmm31
	vpcmpnleb k6{k4}, xmm2, oword [0x216]
	vpcmpnleb k2{k3}, ymm3, ymm6
	vpcmpnleb k6{k2}, zmm0, zmm1
	vpcmpnleb k3, xmm1, oword [eax+1]
	vpcmpnleb k2, xmm3, oword [eax+64]
	vpcmpnleb k2, ymm4, yword [eax+1]
	vpcmpnleb k4, ymm2, yword [eax+64]
	vpcmpnleb k5, zmm6, zword [eax+1]
	vpcmpnleb k6, zmm1, zword [eax+64]
	vpcmpnleb k4, xmm3, [0xaa2]
	vpcmpnleb k4, ymm6, [0xd40]
	vpcmpnleb k7, zmm1, [0x6d1]
