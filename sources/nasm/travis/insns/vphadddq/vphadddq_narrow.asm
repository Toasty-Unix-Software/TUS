	vphadddq xmm5, xmm7
	vphadddq xmm5
	vphadddq xmm6, oword [0xc1e]

%ifdef ERROR
	vphadddq xmm9, xmm11
%endif
