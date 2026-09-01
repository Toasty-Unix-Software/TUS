	cmplesd xmm0, xmm4
	cmplesd xmm3, oword [0xf6b]

%ifdef ERROR
	cmplesd xmm13, xmm9
%endif
