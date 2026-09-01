	setle byte [0x526]
	setle al
	setle ebp
	setle edi

%ifdef ERROR
	setle rcx
	setle rsi
	setle r13b
	setle r8
	setle r12d
	setle r16b
	setle r29
	setle r24d
%endif
