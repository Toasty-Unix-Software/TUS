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

%ifdef ERROR
	vpcmpgtb xmm9, xmm13, xmm8
	vpcmpgtb ymm11, ymm15, ymm9
	vpcmpgtb k5, xmm15, xmm9
	vpcmpgtb k4, ymm8, ymm10
%endif
