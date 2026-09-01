	packuswb mm6, mm0
	packuswb mm7, qword [0x546]
	packuswb xmm4, xmm1
	packuswb xmm2, xmm1

%ifdef ERROR
	packuswb xmm8, xmm15
%endif
