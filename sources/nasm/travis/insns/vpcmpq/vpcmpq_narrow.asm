	vpcmpq k6, xmm7, oword [0x78e], 0xc2
	vpcmpq k2, xmm4, xmm6, 0x45
	vpcmpq k2, ymm6, ymm1, 0xb9
	vpcmpq k6, ymm6, yword [0x45c], 0x38
	vpcmpq k4, zmm1, zword [0x517], 0x1a
	vpcmpq k6, zmm6, zmm3, 0xe9

%ifdef ERROR
	vpcmpq k7, xmm10, xmm14, 0x95
	vpcmpq k1, ymm14, ymm14, 0x90
	vpcmpq k4, zmm11, zmm8, 0xf2
	vpcmpq k2, xmm16, xmm27, 0x4a
	vpcmpq k1, ymm23, ymm28, 0x13
	vpcmpq k7, zmm28, zmm30, 0x33
%endif
