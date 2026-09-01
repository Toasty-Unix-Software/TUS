	movsxb ax, al
	movsxb ax, al
	movsxb bx, dl
	movsxb bx, byte [0x110]
	movsxb eax, byte [0x678]
	movsxb esi, byte [0x598]

%ifdef ERROR
	movsxb rax, cl
	movsxb rsi, cl
	movsxb r13w, r15b
	movsxb r14d, r14b
	movsxb r8, r14b
	movsxb r17w, r23b
	movsxb r26d, r20b
	movsxb r24, r16b
	movsxb rbx, byte [eax+1]
	movsxb rax, byte [eax+64]
	movsxb rbp, [0x36d]
%endif
