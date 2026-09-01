	vpcomb xmm2, xmm2, xmm4, 0x4f
	vpcomb xmm2, xmm4, 0x4f
	vpcomb xmm3, xmm0, oword [0x4d7], 0x7c

%ifdef ERROR
	vpcomb xmm11, xmm13, xmm9, 0xe0
%endif
