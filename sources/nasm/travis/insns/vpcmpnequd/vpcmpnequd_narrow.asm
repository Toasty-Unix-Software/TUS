	vpcmpnequd k7, xmm1, oword [0xd06]
	vpcmpnequd k1, xmm2, xmm0
	vpcmpnequd k6, ymm4, ymm0
	vpcmpnequd k5, ymm4, yword [0x6aa]
	vpcmpnequd k1, zmm5, zmm0
	vpcmpnequd k1, zmm0, zword [0x25d]

%ifdef ERROR
	vpcmpnequd k5, xmm14, xmm10
	vpcmpnequd k5, ymm11, ymm13
	vpcmpnequd k3, zmm11, zmm11
	vpcmpnequd k5, xmm17, xmm19
	vpcmpnequd k6, ymm24, ymm16
	vpcmpnequd k1, zmm19, zmm31
%endif
