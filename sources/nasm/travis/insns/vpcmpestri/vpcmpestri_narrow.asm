	vpcmpestri xmm7, oword [0xa31], 0xab
	vpcmpestri xmm7, xmm0, 0x72

%ifdef ERROR
	vpcmpestri xmm11, xmm11, 0xc
%endif
