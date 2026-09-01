	vpcmpuw k3, xmm2, xmm1, 0x1f
	vpcmpuw k3, xmm6, oword [0xd0d], 0xea
	vpcmpuw k3, ymm6, ymm6, 0x8f
	vpcmpuw k3, ymm0, ymm7, 0xd3
	vpcmpuw k6, zmm5, zword [0x287], 0x2
	vpcmpuw k2, zmm7, zword [0x53d], 0x98

%ifdef ERROR
	vpcmpuw k5, xmm11, xmm9, 0xb5
	vpcmpuw k2, ymm11, ymm11, 0x42
	vpcmpuw k4, zmm12, zmm14, 0xf0
	vpcmpuw k5, xmm19, xmm27, 0x8b
	vpcmpuw k1, ymm26, ymm28, 0x85
	vpcmpuw k1, zmm17, zmm22, 0x22
%endif
