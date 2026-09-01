	vpcmpnltuq k4, xmm2, xmm6
	vpcmpnltuq k6, xmm4, xmm6
	vpcmpnltuq k2, ymm2, ymm6
	vpcmpnltuq k1, ymm1, ymm7
	vpcmpnltuq k6, zmm2, zmm6
	vpcmpnltuq k3, zmm4, zword [0xbcc]

%ifdef ERROR
	vpcmpnltuq k7, xmm14, xmm13
	vpcmpnltuq k4, ymm11, ymm13
	vpcmpnltuq k5, zmm15, zmm14
	vpcmpnltuq k5, xmm23, xmm30
	vpcmpnltuq k4, ymm25, ymm31
	vpcmpnltuq k5, zmm20, zmm29
%endif
