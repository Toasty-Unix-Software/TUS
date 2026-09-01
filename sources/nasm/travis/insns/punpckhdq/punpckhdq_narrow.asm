	punpckhdq mm0, mm5
	punpckhdq mm7, qword [0x876]
	punpckhdq xmm2, xmm0
	punpckhdq xmm6, xmm1

%ifdef ERROR
	punpckhdq xmm11, xmm14
%endif
