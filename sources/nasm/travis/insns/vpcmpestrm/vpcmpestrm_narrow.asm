	vpcmpestrm xmm3, oword [0x4ff], 0xf5
	vpcmpestrm xmm3, xmm5, 0x71

%ifdef ERROR
	vpcmpestrm xmm12, xmm14, 0xe9
%endif
