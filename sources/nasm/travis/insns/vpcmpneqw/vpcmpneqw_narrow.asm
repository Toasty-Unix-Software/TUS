	vpcmpneqw k4, xmm0, xmm1
	vpcmpneqw k7, xmm4, xmm1
	vpcmpneqw k5, ymm4, yword [0x7f7]
	vpcmpneqw k2, ymm3, ymm2
	vpcmpneqw k1, zmm6, zword [0x346]
	vpcmpneqw k1, zmm4, zmm2

%ifdef ERROR
	vpcmpneqw k4, xmm12, xmm13
	vpcmpneqw k4, ymm12, ymm9
	vpcmpneqw k7, zmm11, zmm8
	vpcmpneqw k1, xmm23, xmm21
	vpcmpneqw k6, ymm31, ymm29
	vpcmpneqw k7, zmm17, zmm16
%endif
