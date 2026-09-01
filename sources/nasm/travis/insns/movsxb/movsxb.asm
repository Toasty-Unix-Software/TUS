default rel
	movsxb ax, al
	movsxb ax, al
	movsxb bx, dl
	movsxb bx, byte [0x110]
	movsxb eax, byte [0x678]
	movsxb esi, byte [0x598]
	movsxb rax, cl
	movsxb rsi, cl
	movsxb r13w, r15b
	movsxb r14d, r14b
	movsxb r8, r14b
	movsxb r17w, r23b
	movsxb r26d, r20b
	movsxb r24, r16b
	movsxb bx, byte [eax+1]
	movsxb cx, byte [eax+64]
	movsxb eax, byte [eax+1]
	movsxb edi, byte [eax+64]
	movsxb rbx, byte [eax+1]
	movsxb rax, byte [eax+64]
	movsxb di, [0x872]
	movsxb ecx, [0x85e]
	movsxb rbp, [0x36d]
