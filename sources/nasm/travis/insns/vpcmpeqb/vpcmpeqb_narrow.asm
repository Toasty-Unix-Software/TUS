	vpcmpeqb xmm4, xmm5, xmm0
	vpcmpeqb xmm4, xmm0
	vpcmpeqb xmm2, xmm0, xmm6
	vpcmpeqb ymm7, ymm0, yword [0x4f8]
	vpcmpeqb ymm7, yword [0x4f8]
	vpcmpeqb ymm5, ymm3, ymm3
	vpcmpeqb k3, xmm0, xmm7
	vpcmpeqb k2, xmm0, oword [0xa2f]
	vpcmpeqb k1, ymm4, ymm5
	vpcmpeqb k6, ymm0, yword [0x87a]

%ifdef ERROR
	vpcmpeqb xmm10, xmm14, xmm14
	vpcmpeqb ymm11, ymm12, ymm15
	vpcmpeqb k5, xmm12, xmm8
	vpcmpeqb k1, ymm14, ymm9
%endif
