	cmpunordsd xmm1, xmm2
	cmpunordsd xmm7, oword [0xa84]

%ifdef ERROR
	cmpunordsd xmm10, xmm14
%endif
