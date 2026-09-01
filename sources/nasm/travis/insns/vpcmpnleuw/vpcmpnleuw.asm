default rel
	vpcmpnleuw k6, xmm1, oword [0x860]
	vpcmpnleuw k3, xmm6, oword [0xdea]
	vpcmpnleuw k6, ymm6, ymm7
	vpcmpnleuw k5, ymm7, yword [0xbc8]
	vpcmpnleuw k2, zmm3, zmm1
	vpcmpnleuw k6, zmm4, zmm5
	vpcmpnleuw k1, xmm14, xmm10
	vpcmpnleuw k6, ymm12, ymm14
	vpcmpnleuw k5, zmm11, zmm9
	vpcmpnleuw k5, xmm26, xmm31
	vpcmpnleuw k5, ymm22, ymm27
	vpcmpnleuw k2, zmm29, zmm28
	vpcmpnleuw k2{k3}, xmm3, xmm0
	vpcmpnleuw k2{k4}, ymm3, ymm2
	vpcmpnleuw k2{k1}, zmm4, zmm2
	vpcmpnleuw k2, xmm7, oword [eax+1]
	vpcmpnleuw k7, xmm7, oword [eax+64]
	vpcmpnleuw k7, ymm3, yword [eax+1]
	vpcmpnleuw k4, ymm6, yword [eax+64]
	vpcmpnleuw k1, zmm2, zword [eax+1]
	vpcmpnleuw k6, zmm7, zword [eax+64]
	vpcmpnleuw k3, xmm5, [0xb47]
	vpcmpnleuw k3, ymm0, [0x462]
	vpcmpnleuw k7, zmm3, [0xc4f]
