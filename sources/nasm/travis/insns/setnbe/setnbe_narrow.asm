	setnbe byte [0xb9a]
	setnbe al
	setnbe eax
	setnbe esi

%ifdef ERROR
	setnbe rdi
	setnbe rdx
	setnbe r15b
	setnbe r9
	setnbe r12d
	setnbe r21b
	setnbe r28
	setnbe r19d
%endif
