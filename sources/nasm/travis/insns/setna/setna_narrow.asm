	setna byte [0x586]
	setna al
	setna edi
	setna edx

%ifdef ERROR
	setna rdx
	setna rcx
	setna r11b
	setna r14
	setna r14d
	setna r24b
	setna r16
	setna r23d
%endif
