	punpckhbw mm2, mm4
	punpckhbw mm0, mm1
	punpckhbw xmm4, xmm1
	punpckhbw xmm4, xmm7

%ifdef ERROR
	punpckhbw xmm15, xmm10
%endif
