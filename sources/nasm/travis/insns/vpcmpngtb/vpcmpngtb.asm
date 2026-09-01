default rel
	vpcmpngtb k3, xmm5, xmm5
	vpcmpngtb k2, xmm3, xmm2
	vpcmpngtb k7, ymm4, yword [0xbb5]
	vpcmpngtb k5, ymm0, yword [0xe2a]
	vpcmpngtb k5, zmm7, zmm6
	vpcmpngtb k7, zmm3, zword [0xcb2]
	vpcmpngtb k2, xmm14, xmm14
	vpcmpngtb k3, ymm14, ymm15
	vpcmpngtb k3, zmm10, zmm11
	vpcmpngtb k4, xmm24, xmm28
	vpcmpngtb k5, ymm17, ymm20
	vpcmpngtb k2, zmm26, zmm21
	vpcmpngtb k5{k3}, xmm5, xmm5
	vpcmpngtb k4{k4}, ymm1, ymm4
	vpcmpngtb k2{k1}, zmm4, zmm1
	vpcmpngtb k6, xmm6, oword [eax+1]
	vpcmpngtb k4, xmm7, oword [eax+64]
	vpcmpngtb k3, ymm4, yword [eax+1]
	vpcmpngtb k1, ymm6, yword [eax+64]
	vpcmpngtb k5, zmm5, zword [eax+1]
	vpcmpngtb k3, zmm7, zword [eax+64]
	vpcmpngtb k7, xmm7, [0x2d2]
	vpcmpngtb k2, ymm5, [0x802]
	vpcmpngtb k6, zmm6, [0xdca]
