	vpcmpleuw k4, xmm7, oword [0x150]
	vpcmpleuw k5, xmm2, xmm0
	vpcmpleuw k6, ymm3, yword [0x1ef]
	vpcmpleuw k1, ymm1, yword [0x588]
	vpcmpleuw k6, zmm2, zmm4
	vpcmpleuw k2, zmm0, zword [0xc64]

%ifdef ERROR
	vpcmpleuw k2, xmm13, xmm12
	vpcmpleuw k2, ymm11, ymm15
	vpcmpleuw k7, zmm15, zmm12
	vpcmpleuw k4, xmm28, xmm23
	vpcmpleuw k4, ymm22, ymm24
	vpcmpleuw k2, zmm22, zmm29
%endif
