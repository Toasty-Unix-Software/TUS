	setg al
	setg byte [0x3f2]
	setg ecx
	setg ecx

%ifdef ERROR
	setg rbp
	setg rsi
	setg r13b
	setg r9
	setg r14d
	setg r25b
	setg r30
	setg r25d
%endif
