	ccmpnz 9, byte [0x4f5], dl
	ccmpnz 1, dl, bl
	ccmpnz 14, si, bx
	ccmpnz 3, di, bx
	ccmpnz 14, esi, eax
	ccmpnz 3, dword [0xf77], esi

%ifdef ERROR
	ccmpnz 6, qword [0xbce], rdi
	ccmpnz 13, qword [0x80f], rdi
	ccmpnz 4, r14b, r15b
	ccmpnz 10, r14w, r10w
	ccmpnz 6, r13d, r8d
	ccmpnz 8, r10, r8
	ccmpnz 9, r27b, r23b
	ccmpnz 0, r29w, r19w
	ccmpnz 6, r27d, r26d
	ccmpnz 15, r24, r28
	ccmpnz 3, qword [eax+1], rdi
	ccmpnz 3, qword [eax+64], rsi
	ccmpnz 13, rax, qword [eax+1]
	ccmpnz 11, rbp, qword [eax+64]
	ccmpnz 4, qword [eax+1], -10
	ccmpnz 8, qword [eax+64], 36
	ccmpnz 6, qword [eax+1], -252757675
	ccmpnz 3, qword [eax+64], -262447455
	ccmpnz 7, [0x119], rdx
	ccmpnz 11, rbx, [0xee9]
	ccmpnz 2, [0x924], -100
	ccmpnz 1, [0x190], 41689417
%endif
