	vpcmpltuq k5, xmm0, xmm0
	vpcmpltuq k2, xmm0, xmm4
	vpcmpltuq k7, ymm2, ymm0
	vpcmpltuq k7, ymm6, ymm3
	vpcmpltuq k5, zmm6, zmm2
	vpcmpltuq k6, zmm6, zword [0x7bf]

%ifdef ERROR
	vpcmpltuq k4, xmm13, xmm12
	vpcmpltuq k6, ymm15, ymm12
	vpcmpltuq k7, zmm14, zmm11
	vpcmpltuq k1, xmm20, xmm29
	vpcmpltuq k1, ymm21, ymm22
	vpcmpltuq k3, zmm17, zmm26
%endif
