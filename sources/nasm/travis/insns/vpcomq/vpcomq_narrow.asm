	vpcomq xmm2, xmm5, xmm5, 0x4f
	vpcomq xmm2, xmm5, 0x4f
	vpcomq xmm0, xmm6, oword [0x3ca], 0x71

%ifdef ERROR
	vpcomq xmm11, xmm14, xmm14, 0xaf
%endif
