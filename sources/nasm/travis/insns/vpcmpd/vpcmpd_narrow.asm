	vpcmpd k7, xmm1, oword [0xb83], 0x34
	vpcmpd k3, xmm3, xmm5, 0x61
	vpcmpd k3, ymm2, ymm1, 0xbb
	vpcmpd k7, ymm1, ymm2, 0xf0
	vpcmpd k6, zmm1, zmm1, 0x38
	vpcmpd k4, zmm0, zword [0xddc], 0xcf

%ifdef ERROR
	vpcmpd k1, xmm14, xmm9, 0x1b
	vpcmpd k7, ymm12, ymm12, 0x1e
	vpcmpd k2, zmm12, zmm10, 0xcc
	vpcmpd k7, xmm28, xmm18, 0x1
	vpcmpd k2, ymm16, ymm24, 0x9a
	vpcmpd k7, zmm19, zmm21, 0x87
%endif
