	setbzu eax
	setbzu eax
	setbzu byte [0xb55]
	setbzu al

%ifdef ERROR
	setbzu rcx
	setbzu rdi
	setbzu r11
	setbzu r12d
	setbzu r14b
	setbzu r23
	setbzu r17d
	setbzu r24b
%endif
