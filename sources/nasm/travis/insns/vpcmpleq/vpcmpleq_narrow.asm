	vpcmpleq k1, xmm0, xmm5
	vpcmpleq k6, xmm1, xmm7
	vpcmpleq k5, ymm2, yword [0xa41]
	vpcmpleq k2, ymm4, yword [0xe4c]
	vpcmpleq k4, zmm2, zmm7
	vpcmpleq k1, zmm4, zmm2

%ifdef ERROR
	vpcmpleq k4, xmm12, xmm14
	vpcmpleq k1, ymm10, ymm14
	vpcmpleq k1, zmm12, zmm12
	vpcmpleq k6, xmm31, xmm24
	vpcmpleq k6, ymm17, ymm19
	vpcmpleq k7, zmm21, zmm29
%endif
