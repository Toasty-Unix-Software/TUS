	vdppd xmm5, xmm4, oword [0xd97], 0x46
	vdppd xmm5, oword [0xd97], 0x46
	vdppd xmm3, xmm6, xmm7, 0x36

%ifdef ERROR
	vdppd xmm15, xmm12, xmm9, 0x65
%endif
