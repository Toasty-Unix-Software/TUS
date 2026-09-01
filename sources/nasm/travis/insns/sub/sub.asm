default rel
	sub al, al
	sub byte [0x1f3], cl
	sub word [0x24b], bx
	sub dx, bp
	sub edx, ebp
	sub ebx, ecx
	sub rbx, rcx
	sub rbx, rsi
	sub r14b, r10b
	sub r14w, r11w
	sub r15d, r15d
	sub r13, r13
	sub r26b, r27b
	sub r16w, r24w
	sub r30d, r31d
	sub r24, r18
	sub byte [eax+1], dl
	sub byte [eax+64], bl
	sub word [eax+1], cx
	sub word [eax+64], bx
	sub dword [eax+1], ebp
	sub dword [eax+64], ebx
	sub qword [eax+1], rbp
	sub qword [eax+64], rdx
	sub bl, byte [eax+1]
	sub dl, byte [eax+64]
	sub dx, word [eax+1]
	sub si, word [eax+64]
	sub ebx, dword [eax+1]
	sub edi, dword [eax+64]
	sub rdx, qword [eax+1]
	sub rcx, qword [eax+64]
	sub byte [eax+1], 0x54
	sub byte [eax+64], 0x2c
	sub word [eax+1], -66
	sub word [eax+64], -30
	sub word [eax+1], 0x641a
	sub word [eax+64], 0x8e3f
	sub dword [eax+1], -89
	sub dword [eax+64], -63
	sub dword [eax+1], 0x15fdb47a
	sub dword [eax+64], 0xd202dd9
	sub qword [eax+1], -15
	sub qword [eax+64], -91
	sub qword [eax+1], -391842762
	sub qword [eax+64], -227951707
	sub al, cl, byte [eax+1]
	sub al, dl, byte [eax+64]
	sub si, dx, word [eax+1]
	sub cx, cx, word [eax+64]
	sub edx, esi, dword [eax+1]
	sub edx, eax, dword [eax+64]
	sub rbp, rsi, qword [eax+1]
	sub rbp, rdx, qword [eax+64]
	sub al, byte [eax+1], dl
	sub bl, byte [eax+64], bl
	sub ax, word [eax+1], si
	sub dx, word [eax+64], di
	sub esi, dword [eax+1], esi
	sub esi, dword [eax+64], eax
	sub rbp, qword [eax+1], rdi
	sub rbp, qword [eax+64], rbp
	sub bp, word [eax+1], -89
	sub dx, word [eax+64], 96
	sub ebp, dword [eax+1], -33
	sub esi, dword [eax+64], -26
	sub rbx, qword [eax+1], -12
	sub rdi, qword [eax+64], -55
	sub dl, byte [eax+1], 0xfc
	sub cl, byte [eax+64], 0x5e
	sub bp, word [eax+1], 0x5ea2
	sub si, word [eax+64], 0x3f30
	sub edx, dword [eax+1], 0x14955a0b
	sub eax, dword [eax+64], 0x1d7e895d
	sub rax, qword [eax+1], -507148648
	sub rcx, qword [eax+64], -395403989
	sub [0x924], cl
	sub [0x22b], si
	sub [0x1b5], edi
	sub [0xd9e], rbp
	sub al, [0xda0]
	sub ax, [0xc84]
	sub edi, [0xb8c]
	sub rbp, [0x9e3]
	sub [0xd7b], 0xe7
	sub [0x60a], -37
	sub [0xac3], 0x4e70
	sub [0xe26], 101
	sub [0xafd], 0x263661f5
	sub [0xb18], 54
	sub [0xcc0], -276330528
	sub bl, dl, [0xf72]
	sub ax, bx, [0x604]
	sub esi, ebx, [0xd0f]
	sub rdi, rbx, [0x5b0]
	sub cl, [0xef1], al
	sub bx, [0x125], cx
	sub esi, [0x46e], eax
	sub rbp, [0x4bb], rdi
	sub ax, [0xe29], 82
	sub edx, [0x984], 49
	sub rcx, [0x95b], -97
	sub dl, [0x1bb], 0xb4
	sub bp, [0xe8a], 0xae06
	sub ebx, [0xeb3], 0x29a63948
	sub rbp, [0xc4c], -302261169
