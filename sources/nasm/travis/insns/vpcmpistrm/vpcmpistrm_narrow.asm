	vpcmpistrm xmm6, xmm0, 0x5f
	vpcmpistrm xmm0, xmm5, 0x7c

%ifdef ERROR
	vpcmpistrm xmm10, xmm10, 0x1e
%endif
