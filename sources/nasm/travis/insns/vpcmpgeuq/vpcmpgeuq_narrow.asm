	vpcmpgeuq k2, xmm2, oword [0x75c]
	vpcmpgeuq k6, xmm2, oword [0x81e]
	vpcmpgeuq k4, ymm3, yword [0xbf1]
	vpcmpgeuq k7, ymm2, yword [0x11e]
	vpcmpgeuq k7, zmm2, zmm3
	vpcmpgeuq k6, zmm0, zmm3

%ifdef ERROR
	vpcmpgeuq k5, xmm13, xmm14
	vpcmpgeuq k1, ymm12, ymm11
	vpcmpgeuq k6, zmm15, zmm14
	vpcmpgeuq k3, xmm18, xmm16
	vpcmpgeuq k1, ymm23, ymm27
	vpcmpgeuq k3, zmm31, zmm29
%endif
