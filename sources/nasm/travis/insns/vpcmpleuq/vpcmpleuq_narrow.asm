	vpcmpleuq k1, xmm4, oword [0x430]
	vpcmpleuq k7, xmm6, xmm1
	vpcmpleuq k2, ymm0, ymm1
	vpcmpleuq k5, ymm7, ymm4
	vpcmpleuq k3, zmm2, zmm0
	vpcmpleuq k6, zmm6, zword [0xe44]

%ifdef ERROR
	vpcmpleuq k7, xmm14, xmm10
	vpcmpleuq k1, ymm11, ymm12
	vpcmpleuq k4, zmm11, zmm15
	vpcmpleuq k5, xmm29, xmm24
	vpcmpleuq k4, ymm22, ymm26
	vpcmpleuq k6, zmm23, zmm28
%endif
