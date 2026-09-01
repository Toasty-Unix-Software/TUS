	setpezu edx
	setpezu ecx
	setpezu byte [0x76b]
	setpezu al

%ifdef ERROR
	setpezu rbp
	setpezu rcx
	setpezu r11
	setpezu r11d
	setpezu r9b
	setpezu r30
	setpezu r27d
	setpezu r16b
%endif
