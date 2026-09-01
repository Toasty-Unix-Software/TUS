	vpcmpleb k5, xmm2, xmm0
	vpcmpleb k4, xmm3, xmm5
	vpcmpleb k3, ymm4, ymm0
	vpcmpleb k6, ymm4, ymm5
	vpcmpleb k5, zmm2, zmm6
	vpcmpleb k5, zmm2, zmm1

%ifdef ERROR
	vpcmpleb k1, xmm15, xmm9
	vpcmpleb k4, ymm15, ymm11
	vpcmpleb k1, zmm10, zmm8
	vpcmpleb k7, xmm19, xmm20
	vpcmpleb k7, ymm16, ymm24
	vpcmpleb k5, zmm21, zmm19
%endif
