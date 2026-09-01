default rel
	ctestnc 8, al, cl
	ctestnc 12, byte [0x97d], dl
	ctestnc 10, word [0x235], bx
	ctestnc 0, word [0x508], ax
	ctestnc 3, dword [0x9a0], ebp
	ctestnc 8, edx, eax
	ctestnc 11, rdx, rbx
	ctestnc 10, qword [0xcfd], rcx
	ctestnc 15, r13b, r14b
	ctestnc 7, r8w, r14w
	ctestnc 4, r10d, r12d
	ctestnc 10, r9, r14
	ctestnc 3, r24b, r26b
	ctestnc 6, r16w, r19w
	ctestnc 6, r16d, r31d
	ctestnc 6, r21, r19
	ctestnc 6, byte [eax+1], dl
	ctestnc 2, byte [eax+64], al
	ctestnc 4, word [eax+1], di
	ctestnc 6, word [eax+64], ax
	ctestnc 12, dword [eax+1], edx
	ctestnc 2, dword [eax+64], ebx
	ctestnc 8, qword [eax+1], rbx
	ctestnc 8, qword [eax+64], rbp
	ctestnc 13, byte [eax+1], 0x77
	ctestnc 12, byte [eax+64], 0xed
	ctestnc 10, word [eax+1], 0x401a
	ctestnc 12, word [eax+64], 0xe6b2
	ctestnc 6, dword [eax+1], 0x6675399
	ctestnc 11, dword [eax+64], 0x3eb370ca
	ctestnc 1, qword [eax+1], 534467702
	ctestnc 15, qword [eax+64], -258542703
	ctestnc 4, [0xcd8], al
	ctestnc 2, [0x7fa], si
	ctestnc 2, [0x5ad], esi
	ctestnc 6, [0xccd], rax
	ctestnc 8, [0xf8b], 0xeb
	ctestnc 2, [0xb5e], 0xc2eb
	ctestnc 5, [0x18d], 0x3e5e8eaa
	ctestnc 7, [0x3d8], -89677070
