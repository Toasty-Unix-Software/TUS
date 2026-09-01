default rel
	ctestbe 4, byte [0xce0], al
	ctestbe 4, dl, bl
	ctestbe 5, word [0xedd], ax
	ctestbe 12, si, ax
	ctestbe 8, eax, edx
	ctestbe 0, edi, esi
	ctestbe 7, qword [0x31d], rbp
	ctestbe 3, qword [0xb3d], rbp
	ctestbe 4, r13b, r15b
	ctestbe 1, r14w, r14w
	ctestbe 0, r13d, r14d
	ctestbe 11, r8, r9
	ctestbe 12, r25b, r28b
	ctestbe 12, r18w, r31w
	ctestbe 2, r18d, r23d
	ctestbe 4, r19, r29
	ctestbe 4, byte [eax+1], dl
	ctestbe 7, byte [eax+64], cl
	ctestbe 12, word [eax+1], cx
	ctestbe 2, word [eax+64], bx
	ctestbe 8, dword [eax+1], edi
	ctestbe 10, dword [eax+64], ebp
	ctestbe 14, qword [eax+1], rdx
	ctestbe 8, qword [eax+64], rcx
	ctestbe 13, byte [eax+1], 0x68
	ctestbe 11, byte [eax+64], 0xa7
	ctestbe 3, word [eax+1], 0xffcd
	ctestbe 2, word [eax+64], 0x4c57
	ctestbe 2, dword [eax+1], 0xd63bb93
	ctestbe 14, dword [eax+64], 0xe391081
	ctestbe 10, qword [eax+1], 27449154
	ctestbe 4, qword [eax+64], 412993705
	ctestbe 12, [0x4a3], cl
	ctestbe 12, [0x773], bx
	ctestbe 13, [0xd6a], edi
	ctestbe 15, [0x810], rax
	ctestbe 10, [0x580], 0x9a
	ctestbe 8, [0x5f8], 0x3eb7
	ctestbe 12, [0x18f], 0x140fb0a
	ctestbe 6, [0xb90], -426887663
