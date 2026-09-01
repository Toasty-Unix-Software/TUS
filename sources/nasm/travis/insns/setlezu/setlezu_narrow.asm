	setlezu esi
	setlezu ebp
	setlezu byte [0x204]
	setlezu bl

%ifdef ERROR
	setlezu rsi
	setlezu rax
	setlezu r13
	setlezu r10d
	setlezu r12b
	setlezu r28
	setlezu r22d
	setlezu r29b
%endif
