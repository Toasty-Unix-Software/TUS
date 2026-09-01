	pmaddwd mm1, mm5
	pmaddwd mm1, qword [0xdfb]
	pmaddwd xmm5, oword [0x9bf]
	pmaddwd xmm0, xmm1

%ifdef ERROR
	pmaddwd xmm14, xmm12
%endif
