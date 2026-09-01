default rel
	ctestc 10, bl, al
	ctestc 12, bl, al
	ctestc 15, dx, ax
	ctestc 14, word [0xe16], bx
	ctestc 2, esi, edi
	ctestc 4, ebp, edx
	ctestc 14, rcx, rax
	ctestc 1, qword [0x570], rcx
	ctestc 2, r9b, r8b
	ctestc 13, r15w, r8w
	ctestc 5, r11d, r15d
	ctestc 12, r8, r12
	ctestc 6, r28b, r23b
	ctestc 7, r21w, r24w
	ctestc 9, r20d, r27d
	ctestc 0, r18, r30
	ctestc 10, byte [eax+1], dl
	ctestc 10, byte [eax+64], al
	ctestc 13, word [eax+1], ax
	ctestc 14, word [eax+64], bx
	ctestc 1, dword [eax+1], ebx
	ctestc 1, dword [eax+64], edi
	ctestc 3, qword [eax+1], rsi
	ctestc 5, qword [eax+64], rcx
	ctestc 5, byte [eax+1], 0xc7
	ctestc 2, byte [eax+64], 0x47
	ctestc 11, word [eax+1], 0xcc83
	ctestc 8, word [eax+64], 0x5af8
	ctestc 10, dword [eax+1], 0x185f5191
	ctestc 15, dword [eax+64], 0x3e1a7eab
	ctestc 2, qword [eax+1], -346250776
	ctestc 12, qword [eax+64], 437496154
	ctestc 10, [0x824], al
	ctestc 13, [0xd51], dx
	ctestc 7, [0x7a5], ebx
	ctestc 7, [0x2ad], rsi
	ctestc 0, [0x559], 0x59
	ctestc 1, [0x6d6], 0xd21d
	ctestc 2, [0x238], 0x1fbd18f9
	ctestc 10, [0x9a8], 349171076
