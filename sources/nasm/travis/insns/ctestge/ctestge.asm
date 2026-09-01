default rel
	ctestge 5, dl, al
	ctestge 8, dl, cl
	ctestge 3, di, ax
	ctestge 10, word [0xc28], dx
	ctestge 3, edi, ecx
	ctestge 5, ebx, ecx
	ctestge 9, qword [0x6ce], rcx
	ctestge 2, qword [0x20c], rbx
	ctestge 11, r9b, r9b
	ctestge 4, r9w, r8w
	ctestge 4, r12d, r10d
	ctestge 13, r11, r11
	ctestge 15, r20b, r30b
	ctestge 1, r25w, r24w
	ctestge 12, r26d, r28d
	ctestge 10, r17, r27
	ctestge 13, byte [eax+1], bl
	ctestge 0, byte [eax+64], dl
	ctestge 2, word [eax+1], si
	ctestge 7, word [eax+64], di
	ctestge 14, dword [eax+1], ebp
	ctestge 14, dword [eax+64], edi
	ctestge 2, qword [eax+1], rcx
	ctestge 7, qword [eax+64], rbx
	ctestge 11, byte [eax+1], 0x67
	ctestge 7, byte [eax+64], 0x3c
	ctestge 12, word [eax+1], 0x2afc
	ctestge 1, word [eax+64], 0xd0d1
	ctestge 5, dword [eax+1], 0x2d5a41d9
	ctestge 2, dword [eax+64], 0x3d013cea
	ctestge 2, qword [eax+1], 376208280
	ctestge 6, qword [eax+64], -4161025
	ctestge 12, [0x789], dl
	ctestge 5, [0xe94], di
	ctestge 6, [0x39e], esi
	ctestge 6, [0xa4e], rbp
	ctestge 12, [0xc4c], 0xf1
	ctestge 0, [0xf05], 0xc3a6
	ctestge 5, [0xf4d], 0x34f7c5a4
	ctestge 1, [0xd63], -304702834
