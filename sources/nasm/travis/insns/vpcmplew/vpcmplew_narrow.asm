	vpcmplew k6, xmm6, xmm5
	vpcmplew k2, xmm1, xmm4
	vpcmplew k3, ymm3, yword [0x6ca]
	vpcmplew k3, ymm0, yword [0x7df]
	vpcmplew k5, zmm2, zword [0x6c2]
	vpcmplew k4, zmm0, zmm3

%ifdef ERROR
	vpcmplew k4, xmm13, xmm9
	vpcmplew k2, ymm11, ymm13
	vpcmplew k4, zmm13, zmm11
	vpcmplew k1, xmm22, xmm17
	vpcmplew k6, ymm30, ymm31
	vpcmplew k5, zmm30, zmm27
%endif
