	cmppd xmm4, xmm3, 0xe7
	cmppd xmm6, xmm4, 0x11

%ifdef ERROR
	cmppd xmm14, xmm10, 0x7f
%endif
