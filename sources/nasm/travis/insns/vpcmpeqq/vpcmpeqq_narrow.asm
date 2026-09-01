	vpcmpeqq xmm2, xmm2, xmm1
	vpcmpeqq xmm2, xmm1
	vpcmpeqq xmm3, xmm3, xmm7
	vpcmpeqq ymm1, ymm4, ymm1
	vpcmpeqq ymm1, ymm1
	vpcmpeqq ymm2, ymm0, ymm6
	vpcmpeqq k1, xmm6, xmm5
	vpcmpeqq k4, xmm2, xmm3
	vpcmpeqq k4, ymm0, ymm5
	vpcmpeqq k1, ymm0, ymm7

%ifdef ERROR
	vpcmpeqq xmm12, xmm13, xmm11
	vpcmpeqq ymm8, ymm10, ymm11
	vpcmpeqq k4, xmm8, xmm8
	vpcmpeqq k5, ymm12, ymm9
%endif
