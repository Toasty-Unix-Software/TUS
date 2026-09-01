default rel
	vpcmpnequb k2, xmm5, xmm0
	vpcmpnequb k5, xmm6, xmm3
	vpcmpnequb k7, ymm6, ymm6
	vpcmpnequb k4, ymm0, ymm1
	vpcmpnequb k1, zmm5, zword [0xbea]
	vpcmpnequb k7, zmm4, zword [0x576]
	vpcmpnequb k2, xmm9, xmm10
	vpcmpnequb k1, ymm12, ymm14
	vpcmpnequb k6, zmm8, zmm9
	vpcmpnequb k7, xmm20, xmm29
	vpcmpnequb k7, ymm17, ymm20
	vpcmpnequb k3, zmm27, zmm23
	vpcmpnequb k2{k7}, xmm5, xmm6
	vpcmpnequb k7{k1}, ymm1, ymm2
	vpcmpnequb k2{k4}, zmm4, zmm0
	vpcmpnequb k1, xmm3, oword [eax+1]
	vpcmpnequb k2, xmm3, oword [eax+64]
	vpcmpnequb k3, ymm7, yword [eax+1]
	vpcmpnequb k6, ymm7, yword [eax+64]
	vpcmpnequb k7, zmm4, zword [eax+1]
	vpcmpnequb k4, zmm3, zword [eax+64]
	vpcmpnequb k2, xmm2, [0x94c]
	vpcmpnequb k2, ymm1, [0xc6f]
	vpcmpnequb k7, zmm5, [0xb47]
