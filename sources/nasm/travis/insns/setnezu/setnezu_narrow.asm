	setnezu edx
	setnezu ecx
	setnezu dl
	setnezu byte [0xb42]

%ifdef ERROR
	setnezu rsi
	setnezu rdi
	setnezu r13
	setnezu r12d
	setnezu r8b
	setnezu r29
	setnezu r29d
	setnezu r18b
%endif
