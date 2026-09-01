default rel
	vpcmpgtud k2, xmm0, xmm5
	vpcmpgtud k6, xmm1, xmm3
	vpcmpgtud k2, ymm4, ymm3
	vpcmpgtud k2, ymm3, ymm3
	vpcmpgtud k1, zmm5, zmm3
	vpcmpgtud k2, zmm0, zword [0xd34]
	vpcmpgtud k4, xmm8, xmm10
	vpcmpgtud k6, ymm14, ymm12
	vpcmpgtud k4, zmm12, zmm12
	vpcmpgtud k4, xmm28, xmm28
	vpcmpgtud k7, ymm21, ymm28
	vpcmpgtud k4, zmm30, zmm25
	vpcmpgtud k7{k4}, xmm3, oword [0xb16]
	vpcmpgtud k2{k7}, ymm1, ymm2
	vpcmpgtud k4{k1}, zmm2, zword [0xb3a]
	vpcmpgtud k5, xmm2, oword [eax+1]
	vpcmpgtud k2, xmm7, oword [eax+64]
	vpcmpgtud k6, ymm4, yword [eax+1]
	vpcmpgtud k6, ymm6, yword [eax+64]
	vpcmpgtud k5, zmm5, zword [eax+1]
	vpcmpgtud k3, zmm1, zword [eax+64]
	vpcmpgtud k7, xmm6, [0xe45]
	vpcmpgtud k6, ymm6, [0xa9e]
	vpcmpgtud k7, zmm6, [0xf1d]
