	vpcmpnleud k2, xmm5, xmm3
	vpcmpnleud k4, xmm4, oword [0x276]
	vpcmpnleud k5, ymm3, ymm1
	vpcmpnleud k2, ymm6, ymm1
	vpcmpnleud k1, zmm5, zword [0x18b]
	vpcmpnleud k7, zmm7, zmm0

%ifdef ERROR
	vpcmpnleud k1, xmm14, xmm10
	vpcmpnleud k4, ymm13, ymm14
	vpcmpnleud k4, zmm8, zmm11
	vpcmpnleud k1, xmm20, xmm28
	vpcmpnleud k4, ymm21, ymm19
	vpcmpnleud k1, zmm27, zmm26
%endif
