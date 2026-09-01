	pslld mm3, mm1
	pslld mm6, mm5
	pslld mm1, 0x13
	pslld mm0, 0x51
	pslld xmm1, xmm7
	pslld xmm0, xmm4
	pslld xmm3, 0x2a
	pslld xmm3, 0x71

%ifdef ERROR
	pslld xmm12, xmm9
	pslld xmm15, 0x66
%endif
