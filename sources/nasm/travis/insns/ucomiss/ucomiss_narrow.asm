	ucomiss xmm1, xmm4
	ucomiss xmm5, dword [0xaa0]

%ifdef ERROR
	ucomiss xmm15, xmm14
%endif
