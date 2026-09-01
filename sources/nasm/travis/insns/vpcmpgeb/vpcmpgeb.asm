default rel
	vpcmpgeb k7, xmm0, xmm6
	vpcmpgeb k5, xmm7, oword [0xc19]
	vpcmpgeb k6, ymm6, ymm2
	vpcmpgeb k1, ymm6, ymm7
	vpcmpgeb k4, zmm6, zmm5
	vpcmpgeb k2, zmm6, zmm6
	vpcmpgeb k4, xmm12, xmm13
	vpcmpgeb k7, ymm8, ymm8
	vpcmpgeb k5, zmm15, zmm10
	vpcmpgeb k7, xmm31, xmm26
	vpcmpgeb k7, ymm21, ymm30
	vpcmpgeb k3, zmm18, zmm21
	vpcmpgeb k6{k2}, xmm1, xmm5
	vpcmpgeb k1{k7}, ymm3, ymm2
	vpcmpgeb k3{k4}, zmm5, zword [0xad9]
	vpcmpgeb k2, xmm1, oword [eax+1]
	vpcmpgeb k4, xmm7, oword [eax+64]
	vpcmpgeb k3, ymm6, yword [eax+1]
	vpcmpgeb k2, ymm5, yword [eax+64]
	vpcmpgeb k1, zmm4, zword [eax+1]
	vpcmpgeb k7, zmm1, zword [eax+64]
	vpcmpgeb k7, xmm1, [0x9e5]
	vpcmpgeb k4, ymm5, [0xc4c]
	vpcmpgeb k6, zmm2, [0xe4f]
