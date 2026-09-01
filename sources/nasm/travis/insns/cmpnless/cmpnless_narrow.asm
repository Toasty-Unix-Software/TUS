	cmpnless xmm3, xmm4
	cmpnless xmm0, dword [0x8ce]

%ifdef ERROR
	cmpnless xmm11, xmm12
%endif
