	extrq xmm3, 0x51, 0x73
	extrq xmm0, 0x3e, 0x3d
	extrq xmm1, xmm0
	extrq xmm3, xmm4

%ifdef ERROR
	extrq xmm12, 0x18, 0x68
	extrq xmm13, xmm13
%endif
