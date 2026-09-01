	divss xmm6, dword [0x2e2]
	divss xmm7, dword [0x5e8]

%ifdef ERROR
	divss xmm12, xmm10
%endif
