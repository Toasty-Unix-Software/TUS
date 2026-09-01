	vpcmpngtq k2, xmm4, xmm0
	vpcmpngtq k4, xmm0, xmm6
	vpcmpngtq k4, ymm7, ymm2
	vpcmpngtq k6, ymm4, yword [0x9ed]
	vpcmpngtq k3, zmm6, zword [0xaa2]
	vpcmpngtq k4, zmm5, zmm2

%ifdef ERROR
	vpcmpngtq k3, xmm11, xmm14
	vpcmpngtq k2, ymm10, ymm12
	vpcmpngtq k7, zmm10, zmm15
	vpcmpngtq k1, xmm16, xmm19
	vpcmpngtq k7, ymm28, ymm16
	vpcmpngtq k1, zmm23, zmm16
%endif
