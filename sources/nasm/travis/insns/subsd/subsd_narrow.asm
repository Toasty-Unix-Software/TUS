	subsd xmm0, xmm6
	subsd xmm5, qword [0x9c5]

%ifdef ERROR
	subsd xmm14, xmm13
%endif
