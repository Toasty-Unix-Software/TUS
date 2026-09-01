	setc bl
	setc byte [0x7b4]
	setc ebp
	setc eax

%ifdef ERROR
	setc rdx
	setc rbp
	setc r12b
	setc r9
	setc r12d
	setc r18b
	setc r25
	setc r20d
%endif
