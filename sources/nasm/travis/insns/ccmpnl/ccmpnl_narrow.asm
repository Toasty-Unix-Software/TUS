	ccmpnl 11, bl, al
	ccmpnl 4, al, dl
	ccmpnl 12, word [0xdb5], si
	ccmpnl 2, di, bx
	ccmpnl 1, ebx, esi
	ccmpnl 2, edx, esi

%ifdef ERROR
	ccmpnl 11, qword [0x6e1], rsi
	ccmpnl 4, qword [0x3ae], rcx
	ccmpnl 7, r14b, r13b
	ccmpnl 8, r14w, r15w
	ccmpnl 7, r10d, r14d
	ccmpnl 5, r9, r13
	ccmpnl 4, r29b, r24b
	ccmpnl 13, r25w, r30w
	ccmpnl 15, r16d, r16d
	ccmpnl 7, r25, r27
	ccmpnl 4, qword [eax+1], rax
	ccmpnl 5, qword [eax+64], rcx
	ccmpnl 12, rbx, qword [eax+1]
	ccmpnl 2, rsi, qword [eax+64]
	ccmpnl 13, qword [eax+1], -121
	ccmpnl 0, qword [eax+64], 36
	ccmpnl 7, qword [eax+1], 23818694
	ccmpnl 9, qword [eax+64], -307914047
	ccmpnl 15, [0x3c4], rbx
	ccmpnl 11, rdx, [0x6d8]
	ccmpnl 9, [0x280], 105
	ccmpnl 1, [0xa0f], 493604984
%endif
