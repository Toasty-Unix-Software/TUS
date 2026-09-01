default rel
	ctestz 1, dl, al
	ctestz 6, dl, cl
	ctestz 4, word [0x516], dx
	ctestz 15, ax, bp
	ctestz 7, dword [0x15b], esi
	ctestz 14, ebx, edi
	ctestz 5, qword [0xfb8], rbp
	ctestz 12, rax, rbp
	ctestz 6, r11b, r14b
	ctestz 12, r14w, r14w
	ctestz 14, r10d, r14d
	ctestz 4, r8, r11
	ctestz 0, r26b, r31b
	ctestz 13, r29w, r27w
	ctestz 3, r21d, r16d
	ctestz 15, r28, r16
	ctestz 14, byte [eax+1], bl
	ctestz 6, byte [eax+64], bl
	ctestz 7, word [eax+1], bx
	ctestz 6, word [eax+64], bp
	ctestz 10, dword [eax+1], ebx
	ctestz 13, dword [eax+64], eax
	ctestz 14, qword [eax+1], rax
	ctestz 0, qword [eax+64], rbp
	ctestz 12, byte [eax+1], 0x25
	ctestz 2, byte [eax+64], 0xaf
	ctestz 8, word [eax+1], 0x5ea4
	ctestz 10, word [eax+64], 0xa266
	ctestz 2, dword [eax+1], 0x94fbafa
	ctestz 15, dword [eax+64], 0x1518c37d
	ctestz 8, qword [eax+1], -115784371
	ctestz 1, qword [eax+64], 494357808
	ctestz 3, [0xa7f], bl
	ctestz 4, [0x6d7], bp
	ctestz 10, [0xea2], ebp
	ctestz 3, [0xed0], rcx
	ctestz 2, [0xe7f], 0xb7
	ctestz 1, [0x921], 0x91d1
	ctestz 5, [0xc90], 0x1ab15d4c
	ctestz 4, [0xd96], 526714065
