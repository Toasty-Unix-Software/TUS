	cmpneqss xmm7, xmm0
	cmpneqss xmm5, dword [0xcae]

%ifdef ERROR
	cmpneqss xmm9, xmm14
%endif
