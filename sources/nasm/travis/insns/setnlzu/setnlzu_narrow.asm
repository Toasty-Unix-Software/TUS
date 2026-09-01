	setnlzu edx
	setnlzu ecx
	setnlzu al
	setnlzu byte [0xc6f]

%ifdef ERROR
	setnlzu rdx
	setnlzu rdi
	setnlzu r15
	setnlzu r12d
	setnlzu r12b
	setnlzu r16
	setnlzu r29d
	setnlzu r16b
%endif
