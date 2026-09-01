	vroundpd xmm3, oword [0x827], 0x40
	vroundpd xmm4, xmm3, 0x51
	vroundpd ymm1, ymm3, 0x9a
	vroundpd ymm3, yword [0x434], 0x11

%ifdef ERROR
	vroundpd xmm13, xmm14, 0x66
	vroundpd ymm9, ymm15, 0xa9
%endif
