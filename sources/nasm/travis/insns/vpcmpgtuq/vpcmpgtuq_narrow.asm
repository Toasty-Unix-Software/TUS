	vpcmpgtuq k2, xmm4, xmm3
	vpcmpgtuq k3, xmm0, xmm3
	vpcmpgtuq k1, ymm5, ymm0
	vpcmpgtuq k6, ymm2, yword [0xb6e]
	vpcmpgtuq k3, zmm3, zword [0x4ba]
	vpcmpgtuq k1, zmm1, zmm2

%ifdef ERROR
	vpcmpgtuq k4, xmm12, xmm10
	vpcmpgtuq k5, ymm15, ymm14
	vpcmpgtuq k6, zmm8, zmm9
	vpcmpgtuq k3, xmm16, xmm29
	vpcmpgtuq k3, ymm30, ymm27
	vpcmpgtuq k1, zmm23, zmm27
%endif
