	punpcklbw mm5, mm5
	punpcklbw mm6, qword [0xc84]
	punpcklbw xmm2, oword [0x204]
	punpcklbw xmm2, xmm4

%ifdef ERROR
	punpcklbw xmm15, xmm15
%endif
