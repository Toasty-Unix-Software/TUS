	vpextrq rdx, xmm5, 0x13
	vpextrq rbp, xmm6, 0x5e

%ifdef ERROR
	vpextrq r15, xmm13, 0xdd
	vpextrq r28, xmm16, 0xbd
%endif
