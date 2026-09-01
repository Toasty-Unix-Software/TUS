	pmovzxwq xmm6, xmm5
	pmovzxwq xmm2, dword [0x2b8]

%ifdef ERROR
	pmovzxwq xmm14, xmm8
%endif
