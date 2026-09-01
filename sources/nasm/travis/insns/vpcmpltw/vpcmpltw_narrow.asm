	vpcmpltw k2, xmm2, xmm7
	vpcmpltw k6, xmm1, oword [0x315]
	vpcmpltw k4, ymm1, ymm4
	vpcmpltw k1, ymm4, ymm5
	vpcmpltw k5, zmm1, zmm2
	vpcmpltw k7, zmm0, zword [0xc76]

%ifdef ERROR
	vpcmpltw k4, xmm8, xmm14
	vpcmpltw k7, ymm13, ymm13
	vpcmpltw k7, zmm11, zmm14
	vpcmpltw k6, xmm29, xmm30
	vpcmpltw k6, ymm29, ymm19
	vpcmpltw k2, zmm23, zmm24
%endif
