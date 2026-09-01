	vpcmpgtd xmm5, xmm7, xmm1
	vpcmpgtd xmm5, xmm1
	vpcmpgtd xmm2, xmm2, oword [0x566]
	vpcmpgtd ymm7, ymm7, ymm5
	vpcmpgtd ymm7, ymm5
	vpcmpgtd ymm1, ymm4, ymm2
	vpcmpgtd k4, xmm0, oword [0xcf6]
	vpcmpgtd k3, xmm3, oword [0x997]
	vpcmpgtd k4, ymm5, ymm2
	vpcmpgtd k6, ymm4, yword [0x2b5]

%ifdef ERROR
	vpcmpgtd xmm9, xmm10, xmm13
	vpcmpgtd ymm12, ymm9, ymm9
	vpcmpgtd k7, xmm8, xmm15
	vpcmpgtd k4, ymm9, ymm11
%endif
