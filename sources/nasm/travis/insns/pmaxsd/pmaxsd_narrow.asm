	pmaxsd xmm0, xmm4
	pmaxsd xmm4, oword [0x15b]

%ifdef ERROR
	pmaxsd xmm13, xmm14
%endif
