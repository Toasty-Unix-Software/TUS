default rel
	vpcmpgtb xmm5, xmm0, oword [0x896]
	vpcmpgtb xmm5, oword [0x896]
	vpcmpgtb xmm4, xmm0, xmm6
	vpcmpgtb ymm4, ymm4, ymm6
	vpcmpgtb ymm4, ymm6
	vpcmpgtb ymm6, ymm3, ymm3
	vpcmpgtb k3, xmm2, oword [0x3d8]
	vpcmpgtb k4, xmm1, xmm3
	vpcmpgtb k2, ymm7, ymm1
	vpcmpgtb k3, ymm4, yword [0x3f5]
	vpcmpgtb xmm9, xmm13, xmm8
	vpcmpgtb ymm11, ymm15, ymm9
	vpcmpgtb k5, xmm15, xmm9
	vpcmpgtb k4, ymm8, ymm10
	vpcmpgtb k1{k2}, xmm2, xmm7
	vpcmpgtb k1{k5}, ymm7, ymm4
	vpcmpgtb k4{k1}, zmm5, zword [0xf86]
	vpcmpgtb xmm4, xmm1, oword [eax+1]
	vpcmpgtb xmm3, xmm0, oword [eax+64]
	vpcmpgtb ymm1, ymm5, yword [eax+1]
	vpcmpgtb ymm2, ymm5, yword [eax+64]
	vpcmpgtb k7, xmm3, oword [eax+1]
	vpcmpgtb k1, xmm3, oword [eax+64]
	vpcmpgtb k6, ymm0, yword [eax+1]
	vpcmpgtb k1, ymm5, yword [eax+64]
	vpcmpgtb k4, zmm3, zword [eax+1]
	vpcmpgtb k4, zmm0, zword [eax+64]
	vpcmpgtb xmm3, xmm7, [0x69f]
	vpcmpgtb ymm2, ymm2, [0xb78]
	vpcmpgtb k7, xmm6, [0x352]
	vpcmpgtb k4, ymm1, [0x836]
	vpcmpgtb k4, zmm0, [0x8ea]
