	vpcmpgtq xmm0, xmm3, xmm2
	vpcmpgtq xmm0, xmm2
	vpcmpgtq xmm6, xmm0, xmm6
	vpcmpgtq ymm2, ymm4, ymm5
	vpcmpgtq ymm2, ymm5
	vpcmpgtq ymm0, ymm7, ymm2
	vpcmpgtq k1, xmm1, oword [0x68e]
	vpcmpgtq k7, xmm3, oword [0xe6b]
	vpcmpgtq k2, ymm3, yword [0x7ba]
	vpcmpgtq k7, ymm1, ymm6

%ifdef ERROR
	vpcmpgtq xmm8, xmm13, xmm13
	vpcmpgtq ymm13, ymm10, ymm11
	vpcmpgtq k3, xmm8, xmm9
	vpcmpgtq k2, ymm11, ymm14
%endif
