default rel
	ctestng 2, cl, al
	ctestng 7, dl, bl
	ctestng 8, dx, cx
	ctestng 5, di, ax
	ctestng 13, esi, edi
	ctestng 2, dword [0x4a9], ecx
	ctestng 1, rbp, rcx
	ctestng 0, qword [0xeac], rdx
	ctestng 15, r15b, r13b
	ctestng 13, r8w, r8w
	ctestng 7, r13d, r10d
	ctestng 11, r15, r14
	ctestng 14, r30b, r16b
	ctestng 8, r19w, r17w
	ctestng 9, r31d, r26d
	ctestng 10, r22, r24
	ctestng 5, byte [eax+1], al
	ctestng 15, byte [eax+64], cl
	ctestng 4, word [eax+1], dx
	ctestng 13, word [eax+64], dx
	ctestng 15, dword [eax+1], edx
	ctestng 7, dword [eax+64], edx
	ctestng 12, qword [eax+1], rbp
	ctestng 0, qword [eax+64], rbp
	ctestng 15, byte [eax+1], 0xb0
	ctestng 6, byte [eax+64], 0x1
	ctestng 1, word [eax+1], 0x6739
	ctestng 6, word [eax+64], 0xf60f
	ctestng 8, dword [eax+1], 0x25a562d8
	ctestng 13, dword [eax+64], 0x2e222c32
	ctestng 7, qword [eax+1], -461464867
	ctestng 12, qword [eax+64], -114605603
	ctestng 4, [0x7a6], dl
	ctestng 7, [0x5cf], di
	ctestng 1, [0x70e], ecx
	ctestng 10, [0xec9], rax
	ctestng 2, [0x22a], 0x61
	ctestng 1, [0x893], 0xe3b5
	ctestng 8, [0xa57], 0x164c02f7
	ctestng 5, [0xe83], -330602137
