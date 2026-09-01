default rel
	vpcmpnleub k5, xmm3, xmm0
	vpcmpnleub k3, xmm3, xmm3
	vpcmpnleub k4, ymm3, yword [0xd59]
	vpcmpnleub k4, ymm6, ymm5
	vpcmpnleub k1, zmm5, zmm4
	vpcmpnleub k5, zmm4, zword [0xf06]
	vpcmpnleub k5, xmm13, xmm10
	vpcmpnleub k6, ymm9, ymm8
	vpcmpnleub k7, zmm11, zmm12
	vpcmpnleub k5, xmm27, xmm18
	vpcmpnleub k2, ymm26, ymm24
	vpcmpnleub k2, zmm28, zmm19
	vpcmpnleub k2{k2}, xmm3, xmm4
	vpcmpnleub k6{k4}, ymm0, ymm7
	vpcmpnleub k3{k7}, zmm3, zmm4
	vpcmpnleub k6, xmm1, oword [eax+1]
	vpcmpnleub k5, xmm7, oword [eax+64]
	vpcmpnleub k2, ymm4, yword [eax+1]
	vpcmpnleub k1, ymm0, yword [eax+64]
	vpcmpnleub k4, zmm7, zword [eax+1]
	vpcmpnleub k2, zmm4, zword [eax+64]
	vpcmpnleub k1, xmm2, [0xd7c]
	vpcmpnleub k7, ymm1, [0x433]
	vpcmpnleub k7, zmm6, [0xea1]
