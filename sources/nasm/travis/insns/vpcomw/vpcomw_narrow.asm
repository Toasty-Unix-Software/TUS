	vpcomw xmm4, xmm2, xmm5, 0x4f
	vpcomw xmm4, xmm5, 0x4f
	vpcomw xmm1, xmm4, xmm2, 0xb7

%ifdef ERROR
	vpcomw xmm15, xmm9, xmm15, 0xac
%endif
