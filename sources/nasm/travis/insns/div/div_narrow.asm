	div byte [0xd38]
	div byte [0xc46]
	div word [0x965]
	div ax
	div edi
	div dword [0xab5]
	div rsi
	div rsi

%ifdef ERROR
	div r11b
	div r8w
	div r10d
	div r11
	div r16b
	div r30w
	div r20d
	div r26
%endif
