	vpcmpleud k6, xmm6, xmm7
	vpcmpleud k1, xmm5, oword [0xb60]
	vpcmpleud k2, ymm1, ymm0
	vpcmpleud k5, ymm5, ymm3
	vpcmpleud k1, zmm2, zword [0xac1]
	vpcmpleud k2, zmm1, zword [0x8d5]

%ifdef ERROR
	vpcmpleud k4, xmm9, xmm13
	vpcmpleud k2, ymm9, ymm9
	vpcmpleud k5, zmm8, zmm11
	vpcmpleud k4, xmm19, xmm30
	vpcmpleud k1, ymm17, ymm16
	vpcmpleud k4, zmm18, zmm24
%endif
