default rel
	vpcmpltb k4, xmm4, xmm2
	vpcmpltb k7, xmm3, xmm4
	vpcmpltb k3, ymm6, ymm5
	vpcmpltb k2, ymm4, ymm0
	vpcmpltb k3, zmm0, zmm3
	vpcmpltb k5, zmm2, zword [0xa52]
	vpcmpltb k1, xmm14, xmm8
	vpcmpltb k6, ymm13, ymm10
	vpcmpltb k4, zmm13, zmm10
	vpcmpltb k2, xmm27, xmm21
	vpcmpltb k4, ymm29, ymm25
	vpcmpltb k3, zmm22, zmm31
	vpcmpltb k5{k4}, xmm4, xmm4
	vpcmpltb k3{k1}, ymm3, ymm5
	vpcmpltb k7{k2}, zmm4, zword [0x3ac]
	vpcmpltb k6, xmm4, oword [eax+1]
	vpcmpltb k3, xmm1, oword [eax+64]
	vpcmpltb k1, ymm3, yword [eax+1]
	vpcmpltb k5, ymm7, yword [eax+64]
	vpcmpltb k3, zmm7, zword [eax+1]
	vpcmpltb k3, zmm6, zword [eax+64]
	vpcmpltb k4, xmm1, [0xf21]
	vpcmpltb k1, ymm1, [0xd57]
	vpcmpltb k6, zmm7, [0xbc1]
