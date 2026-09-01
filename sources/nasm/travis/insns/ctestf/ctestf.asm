default rel
	ctestf 3, byte [0xf94], dl
	ctestf 14, cl, al
	ctestf 14, bx, bp
	ctestf 11, word [0xcea], bp
	ctestf 4, edx, esi
	ctestf 12, ebp, eax
	ctestf 11, qword [0x5ee], rdx
	ctestf 10, rcx, rsi
	ctestf 12, r15b, r8b
	ctestf 5, r13w, r11w
	ctestf 10, r8d, r8d
	ctestf 7, r8, r15
	ctestf 5, r26b, r23b
	ctestf 0, r28w, r16w
	ctestf 9, r21d, r19d
	ctestf 0, r16, r31
	ctestf 7, byte [eax+1], cl
	ctestf 0, byte [eax+64], dl
	ctestf 7, word [eax+1], bp
	ctestf 3, word [eax+64], ax
	ctestf 5, dword [eax+1], edx
	ctestf 5, dword [eax+64], edi
	ctestf 2, qword [eax+1], rdx
	ctestf 3, qword [eax+64], rax
	ctestf 12, byte [eax+1], 0x5f
	ctestf 2, byte [eax+64], 0x54
	ctestf 13, word [eax+1], 0x28e7
	ctestf 13, word [eax+64], 0x743c
	ctestf 1, dword [eax+1], 0xc508047
	ctestf 5, dword [eax+64], 0x2ace439
	ctestf 6, qword [eax+1], -134059472
	ctestf 15, qword [eax+64], 473355415
	ctestf 12, [0x5bf], al
	ctestf 9, [0x831], cx
	ctestf 15, [0x90a], ebp
	ctestf 8, [0xf03], rbp
	ctestf 8, [0xf53], 0x61
	ctestf 12, [0xce0], 0xdd0
	ctestf 6, [0xf1d], 0xb716fe3
	ctestf 13, [0xa10], 27245153
