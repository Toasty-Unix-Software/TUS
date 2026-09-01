default rel
	ctestno 14, bl, dl
	ctestno 12, byte [0x172], al
	ctestno 10, si, dx
	ctestno 3, word [0x106], si
	ctestno 6, edx, eax
	ctestno 11, dword [0xc8e], esi
	ctestno 4, qword [0x1f6], rbp
	ctestno 2, rcx, rsi
	ctestno 2, r11b, r10b
	ctestno 9, r10w, r13w
	ctestno 0, r10d, r12d
	ctestno 7, r15, r12
	ctestno 9, r22b, r25b
	ctestno 2, r29w, r25w
	ctestno 6, r26d, r20d
	ctestno 5, r25, r21
	ctestno 0, byte [eax+1], al
	ctestno 5, byte [eax+64], dl
	ctestno 4, word [eax+1], di
	ctestno 6, word [eax+64], bx
	ctestno 9, dword [eax+1], edi
	ctestno 9, dword [eax+64], ebp
	ctestno 13, qword [eax+1], rsi
	ctestno 15, qword [eax+64], rcx
	ctestno 3, byte [eax+1], 0x2d
	ctestno 11, byte [eax+64], 0x45
	ctestno 1, word [eax+1], 0xf16d
	ctestno 9, word [eax+64], 0xbe59
	ctestno 2, dword [eax+1], 0x20ff9897
	ctestno 11, dword [eax+64], 0x39c69a85
	ctestno 14, qword [eax+1], -237830615
	ctestno 3, qword [eax+64], -497579771
	ctestno 7, [0x56c], cl
	ctestno 13, [0xe4f], di
	ctestno 7, [0xc85], eax
	ctestno 3, [0x3d6], rax
	ctestno 8, [0xcc7], 0xbc
	ctestno 6, [0x828], 0x6c2
	ctestno 7, [0x470], 0x2f5dd494
	ctestno 11, [0xe4b], 280832863
