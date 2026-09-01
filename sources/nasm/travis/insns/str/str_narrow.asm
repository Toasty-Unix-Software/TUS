	str word [0xc25]
	str word [0x5be]
	str dx
	str ax
	str eax
	str edi

%ifdef ERROR
	str rsi
	str rsi
	str r9w
	str r12d
	str r8
	str r18w
	str r30d
	str r26
%endif
