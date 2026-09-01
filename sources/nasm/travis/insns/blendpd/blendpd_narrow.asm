	blendpd xmm3, xmm6, 0x4b
	blendpd xmm0, xmm2, 0x6f

%ifdef ERROR
	blendpd xmm9, xmm13, 0x55
%endif
