	paddd mm0, mm4
	paddd mm4, qword [0x33f]
	paddd xmm5, xmm4
	paddd xmm7, oword [0x695]

%ifdef ERROR
	paddd xmm15, xmm15
%endif
