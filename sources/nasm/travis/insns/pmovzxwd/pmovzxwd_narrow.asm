	pmovzxwd xmm7, qword [0x1d0]
	pmovzxwd xmm6, qword [0xe88]

%ifdef ERROR
	pmovzxwd xmm9, xmm13
%endif
