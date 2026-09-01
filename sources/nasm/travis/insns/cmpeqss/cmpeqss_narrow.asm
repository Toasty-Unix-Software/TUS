	cmpeqss xmm6, xmm3
	cmpeqss xmm5, dword [0xc9e]

%ifdef ERROR
	cmpeqss xmm8, xmm8
%endif
