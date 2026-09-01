	setnczu eax
	setnczu eax
	setnczu byte [0x4ac]
	setnczu byte [0x95f]

%ifdef ERROR
	setnczu rdi
	setnczu rbx
	setnczu r9
	setnczu r11d
	setnczu r15b
	setnczu r22
	setnczu r26d
	setnczu r31b
%endif
