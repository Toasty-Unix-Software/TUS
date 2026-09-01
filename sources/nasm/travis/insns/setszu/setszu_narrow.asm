	setszu ebx
	setszu ebp
	setszu byte [0x5bd]
	setszu cl

%ifdef ERROR
	setszu rdx
	setszu rcx
	setszu r11
	setszu r13d
	setszu r9b
	setszu r16
	setszu r19d
	setszu r21b
%endif
