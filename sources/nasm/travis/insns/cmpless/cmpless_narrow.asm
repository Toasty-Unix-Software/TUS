	cmpless xmm3, xmm6
	cmpless xmm0, dword [0xad0]

%ifdef ERROR
	cmpless xmm15, xmm14
%endif
