	ccmpe 7, al, bl
	ccmpe 10, al, al
	ccmpe 5, di, si
	ccmpe 5, bx, bp
	ccmpe 3, dword [0x8ec], edx
	ccmpe 5, edi, esi

%ifdef ERROR
	ccmpe 7, rax, rbp
	ccmpe 2, rdi, rdi
	ccmpe 13, r13b, r13b
	ccmpe 0, r11w, r9w
	ccmpe 14, r14d, r12d
	ccmpe 13, r15, r14
	ccmpe 1, r18b, r18b
	ccmpe 3, r28w, r29w
	ccmpe 9, r20d, r21d
	ccmpe 7, r30, r18
	ccmpe 5, qword [eax+1], rsi
	ccmpe 12, qword [eax+64], rdi
	ccmpe 12, rdx, qword [eax+1]
	ccmpe 1, rax, qword [eax+64]
	ccmpe 12, qword [eax+1], 4
	ccmpe 13, qword [eax+64], 95
	ccmpe 9, qword [eax+1], 57510227
	ccmpe 6, qword [eax+64], -492244701
	ccmpe 5, [0x745], rbp
	ccmpe 11, rcx, [0xb78]
	ccmpe 0, [0x6a6], 0
	ccmpe 11, [0x2be], -379526228
%endif
