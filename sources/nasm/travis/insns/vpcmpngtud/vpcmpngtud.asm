default rel
	vpcmpngtud k6, xmm4, oword [0xb74]
	vpcmpngtud k7, xmm1, xmm7
	vpcmpngtud k5, ymm4, yword [0x9aa]
	vpcmpngtud k6, ymm1, ymm4
	vpcmpngtud k2, zmm7, zmm7
	vpcmpngtud k2, zmm3, zword [0x4cc]
	vpcmpngtud k7, xmm9, xmm11
	vpcmpngtud k1, ymm12, ymm9
	vpcmpngtud k4, zmm9, zmm12
	vpcmpngtud k6, xmm19, xmm23
	vpcmpngtud k5, ymm25, ymm25
	vpcmpngtud k1, zmm19, zmm20
	vpcmpngtud k6{k2}, xmm6, xmm7
	vpcmpngtud k6{k5}, ymm7, ymm7
	vpcmpngtud k3{k2}, zmm6, zmm0
	vpcmpngtud k3, xmm0, oword [eax+1]
	vpcmpngtud k7, xmm1, oword [eax+64]
	vpcmpngtud k1, ymm6, yword [eax+1]
	vpcmpngtud k1, ymm6, yword [eax+64]
	vpcmpngtud k2, zmm2, zword [eax+1]
	vpcmpngtud k6, zmm3, zword [eax+64]
	vpcmpngtud k5, xmm2, [0x46f]
	vpcmpngtud k7, ymm0, [0xe04]
	vpcmpngtud k6, zmm2, [0x829]
