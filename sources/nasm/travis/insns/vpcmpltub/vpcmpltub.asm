default rel
	vpcmpltub k1, xmm5, oword [0xa43]
	vpcmpltub k1, xmm7, xmm0
	vpcmpltub k6, ymm4, ymm2
	vpcmpltub k3, ymm6, yword [0xa24]
	vpcmpltub k2, zmm5, zmm6
	vpcmpltub k3, zmm7, zword [0xe67]
	vpcmpltub k2, xmm12, xmm8
	vpcmpltub k3, ymm10, ymm12
	vpcmpltub k6, zmm12, zmm15
	vpcmpltub k5, xmm20, xmm21
	vpcmpltub k3, ymm27, ymm29
	vpcmpltub k1, zmm26, zmm19
	vpcmpltub k3{k5}, xmm2, xmm0
	vpcmpltub k4{k5}, ymm4, ymm6
	vpcmpltub k2{k7}, zmm4, zmm1
	vpcmpltub k1, xmm2, oword [eax+1]
	vpcmpltub k4, xmm5, oword [eax+64]
	vpcmpltub k2, ymm3, yword [eax+1]
	vpcmpltub k4, ymm6, yword [eax+64]
	vpcmpltub k4, zmm7, zword [eax+1]
	vpcmpltub k4, zmm7, zword [eax+64]
	vpcmpltub k3, xmm2, [0xb82]
	vpcmpltub k1, ymm1, [0xe93]
	vpcmpltub k2, zmm0, [0x851]
