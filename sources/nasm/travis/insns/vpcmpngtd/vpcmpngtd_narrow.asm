	vpcmpngtd k4, xmm0, oword [0x8c7]
	vpcmpngtd k6, xmm5, xmm7
	vpcmpngtd k1, ymm5, ymm3
	vpcmpngtd k3, ymm0, ymm0
	vpcmpngtd k6, zmm7, zmm5
	vpcmpngtd k6, zmm6, zword [0x506]

%ifdef ERROR
	vpcmpngtd k6, xmm10, xmm9
	vpcmpngtd k7, ymm15, ymm13
	vpcmpngtd k2, zmm9, zmm14
	vpcmpngtd k2, xmm21, xmm27
	vpcmpngtd k4, ymm26, ymm20
	vpcmpngtd k4, zmm24, zmm22
%endif
