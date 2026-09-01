	vpcmpgew k3, xmm5, oword [0x459]
	vpcmpgew k5, xmm3, xmm0
	vpcmpgew k2, ymm4, ymm5
	vpcmpgew k6, ymm6, yword [0x259]
	vpcmpgew k4, zmm0, zmm1
	vpcmpgew k5, zmm1, zmm5

%ifdef ERROR
	vpcmpgew k2, xmm13, xmm8
	vpcmpgew k4, ymm9, ymm15
	vpcmpgew k3, zmm13, zmm8
	vpcmpgew k3, xmm26, xmm22
	vpcmpgew k2, ymm24, ymm20
	vpcmpgew k7, zmm28, zmm18
%endif
