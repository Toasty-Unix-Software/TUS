	vrangess xmm0, xmm1, xmm7, 0xdf
	vrangess xmm0, xmm7, 0xdf
	vrangess xmm5, xmm6, xmm0, 0x26

%ifdef ERROR
	vrangess xmm8, xmm13, xmm12, 0xb4
	vrangess xmm26, xmm27, xmm27, 0x35
%endif
