	vpcmpnltb k7, xmm7, xmm3
	vpcmpnltb k6, xmm3, oword [0x48a]
	vpcmpnltb k3, ymm6, yword [0xdf9]
	vpcmpnltb k2, ymm1, ymm4
	vpcmpnltb k2, zmm3, zmm6
	vpcmpnltb k6, zmm7, zword [0xf5f]

%ifdef ERROR
	vpcmpnltb k7, xmm14, xmm11
	vpcmpnltb k5, ymm8, ymm12
	vpcmpnltb k4, zmm12, zmm11
	vpcmpnltb k5, xmm17, xmm16
	vpcmpnltb k3, ymm21, ymm30
	vpcmpnltb k5, zmm23, zmm31
%endif
