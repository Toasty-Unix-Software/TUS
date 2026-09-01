	phaddsw mm5, mm4
	phaddsw mm4, mm1
	phaddsw xmm3, xmm3
	phaddsw xmm5, oword [0xc90]

%ifdef ERROR
	phaddsw xmm8, xmm12
%endif
