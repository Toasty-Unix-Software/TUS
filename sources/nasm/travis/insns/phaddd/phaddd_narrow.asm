	phaddd mm0, mm2
	phaddd mm1, qword [0x7f6]
	phaddd xmm5, oword [0x1b8]
	phaddd xmm3, xmm1

%ifdef ERROR
	phaddd xmm9, xmm11
%endif
