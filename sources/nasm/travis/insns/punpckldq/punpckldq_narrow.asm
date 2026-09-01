	punpckldq mm2, mm1
	punpckldq mm1, qword [0x7dd]
	punpckldq xmm4, oword [0xbbc]
	punpckldq xmm4, xmm5

%ifdef ERROR
	punpckldq xmm13, xmm11
%endif
