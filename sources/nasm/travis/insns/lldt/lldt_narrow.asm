	lldt word [0x3f0]
	lldt word [0xee9]
	lldt si
	lldt ax
	lldt edx
	lldt ebp

%ifdef ERROR
	lldt rcx
	lldt rcx
	lldt r12w
	lldt r13d
	lldt r13
	lldt r16w
	lldt r16d
	lldt r18
%endif
