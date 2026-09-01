default rel
	ctestne 7, bl, dl
	ctestne 5, byte [0xf01], al
	ctestne 10, word [0xa59], ax
	ctestne 4, ax, dx
	ctestne 9, ebp, eax
	ctestne 15, ebp, ecx
	ctestne 5, rbx, rax
	ctestne 1, rdi, rdi
	ctestne 8, r15b, r9b
	ctestne 14, r11w, r14w
	ctestne 15, r12d, r14d
	ctestne 13, r12, r12
	ctestne 13, r27b, r31b
	ctestne 7, r31w, r22w
	ctestne 4, r30d, r29d
	ctestne 14, r21, r30
	ctestne 6, byte [eax+1], dl
	ctestne 9, byte [eax+64], al
	ctestne 8, word [eax+1], si
	ctestne 3, word [eax+64], dx
	ctestne 3, dword [eax+1], eax
	ctestne 1, dword [eax+64], eax
	ctestne 9, qword [eax+1], rbx
	ctestne 2, qword [eax+64], rdi
	ctestne 2, byte [eax+1], 0x1b
	ctestne 11, byte [eax+64], 0xef
	ctestne 9, word [eax+1], 0x55f4
	ctestne 10, word [eax+64], 0x64a4
	ctestne 8, dword [eax+1], 0x28e9f797
	ctestne 5, dword [eax+64], 0x15b86d42
	ctestne 6, qword [eax+1], 431879569
	ctestne 9, qword [eax+64], -269126422
	ctestne 3, [0xa03], bl
	ctestne 9, [0x3e1], dx
	ctestne 14, [0x55a], ebp
	ctestne 8, [0x8c3], rdx
	ctestne 9, [0x518], 0x30
	ctestne 1, [0xb6e], 0x6f8b
	ctestne 11, [0xd0c], 0x1d2f624a
	ctestne 7, [0x7a3], -285879460
