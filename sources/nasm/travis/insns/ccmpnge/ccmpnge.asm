default rel
	ccmpnge 6, cl, bl
	ccmpnge 11, cl, bl
	ccmpnge 1, dx, bp
	ccmpnge 2, bx, di
	ccmpnge 11, edx, ebp
	ccmpnge 9, ebx, ecx
	ccmpnge 9, rbx, rsi
	ccmpnge 15, qword [0x1c4], rdi
	ccmpnge 15, r14b, r12b
	ccmpnge 14, r15w, r15w
	ccmpnge 4, r14d, r8d
	ccmpnge 8, r12, r10
	ccmpnge 10, r31b, r18b
	ccmpnge 13, r19w, r29w
	ccmpnge 0, r26d, r24d
	ccmpnge 1, r23, r27
	ccmpnge 6, byte [eax+1], bl
	ccmpnge 8, byte [eax+64], bl
	ccmpnge 1, word [eax+1], si
	ccmpnge 9, word [eax+64], bp
	ccmpnge 5, dword [eax+1], ecx
	ccmpnge 7, dword [eax+64], eax
	ccmpnge 3, qword [eax+1], rcx
	ccmpnge 5, qword [eax+64], rdi
	ccmpnge 4, al, byte [eax+1]
	ccmpnge 9, cl, byte [eax+64]
	ccmpnge 9, cx, word [eax+1]
	ccmpnge 2, si, word [eax+64]
	ccmpnge 8, ebp, dword [eax+1]
	ccmpnge 2, ebx, dword [eax+64]
	ccmpnge 1, rbp, qword [eax+1]
	ccmpnge 10, rbx, qword [eax+64]
	ccmpnge 12, word [eax+1], 32
	ccmpnge 7, word [eax+64], -34
	ccmpnge 11, dword [eax+1], -126
	ccmpnge 5, dword [eax+64], 74
	ccmpnge 8, qword [eax+1], -10
	ccmpnge 10, qword [eax+64], 126
	ccmpnge 13, byte [eax+1], 0x73
	ccmpnge 12, byte [eax+64], 0x86
	ccmpnge 2, word [eax+1], 0x81d1
	ccmpnge 1, word [eax+64], 0x1ad6
	ccmpnge 0, dword [eax+1], 0x3904fc99
	ccmpnge 13, dword [eax+64], 0x2e338ccc
	ccmpnge 10, qword [eax+1], 424090048
	ccmpnge 5, qword [eax+64], -460094512
	ccmpnge 10, [0x329], bl
	ccmpnge 10, [0xcee], dx
	ccmpnge 3, [0x594], esi
	ccmpnge 5, [0x5fb], rax
	ccmpnge 8, bl, [0xc37]
	ccmpnge 7, ax, [0x6b4]
	ccmpnge 8, edi, [0x1ee]
	ccmpnge 13, rbp, [0x2e0]
	ccmpnge 14, [0x597], -118
	ccmpnge 6, [0x21c], 77
	ccmpnge 7, [0x3e7], 116
	ccmpnge 2, [0x677], 0x6c
	ccmpnge 0, [0x82a], 0xf418
	ccmpnge 13, [0x554], 0x2bc03842
	ccmpnge 13, [0x562], 441775119
