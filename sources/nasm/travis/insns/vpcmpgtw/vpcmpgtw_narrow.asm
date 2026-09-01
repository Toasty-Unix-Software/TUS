	vpcmpgtw xmm3, xmm7, oword [0x658]
	vpcmpgtw xmm3, oword [0x658]
	vpcmpgtw xmm7, xmm0, oword [0x9d6]
	vpcmpgtw ymm2, ymm0, ymm5
	vpcmpgtw ymm2, ymm5
	vpcmpgtw ymm3, ymm2, ymm5
	vpcmpgtw k4, xmm5, oword [0xb40]
	vpcmpgtw k4, xmm2, xmm2
	vpcmpgtw k2, ymm5, yword [0x2c0]
	vpcmpgtw k3, ymm3, ymm1

%ifdef ERROR
	vpcmpgtw xmm14, xmm9, xmm10
	vpcmpgtw ymm15, ymm11, ymm10
	vpcmpgtw k4, xmm8, xmm15
	vpcmpgtw k4, ymm9, ymm12
%endif
