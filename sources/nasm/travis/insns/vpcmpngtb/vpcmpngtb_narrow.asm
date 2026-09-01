	vpcmpngtb k3, xmm5, xmm5
	vpcmpngtb k2, xmm3, xmm2
	vpcmpngtb k7, ymm4, yword [0xbb5]
	vpcmpngtb k5, ymm0, yword [0xe2a]
	vpcmpngtb k5, zmm7, zmm6
	vpcmpngtb k7, zmm3, zword [0xcb2]

%ifdef ERROR
	vpcmpngtb k2, xmm14, xmm14
	vpcmpngtb k3, ymm14, ymm15
	vpcmpngtb k3, zmm10, zmm11
	vpcmpngtb k4, xmm24, xmm28
	vpcmpngtb k5, ymm17, ymm20
	vpcmpngtb k2, zmm26, zmm21
%endif
