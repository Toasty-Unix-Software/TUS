	cmpnlesd xmm1, oword [0xa77]
	cmpnlesd xmm7, xmm6

%ifdef ERROR
	cmpnlesd xmm14, xmm8
%endif
