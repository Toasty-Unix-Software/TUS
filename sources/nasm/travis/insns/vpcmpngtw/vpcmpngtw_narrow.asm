	vpcmpngtw k3, xmm6, xmm6
	vpcmpngtw k6, xmm5, xmm0
	vpcmpngtw k4, ymm0, ymm4
	vpcmpngtw k4, ymm5, ymm4
	vpcmpngtw k7, zmm4, zword [0xf77]
	vpcmpngtw k6, zmm4, zmm3

%ifdef ERROR
	vpcmpngtw k6, xmm12, xmm15
	vpcmpngtw k2, ymm10, ymm11
	vpcmpngtw k3, zmm13, zmm11
	vpcmpngtw k1, xmm20, xmm18
	vpcmpngtw k7, ymm22, ymm30
	vpcmpngtw k4, zmm19, zmm17
%endif
