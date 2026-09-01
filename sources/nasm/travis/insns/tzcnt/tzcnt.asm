default rel
	tzcnt bx, dx
	tzcnt si, word [0x6f9]
	tzcnt ebp, dword [0x679]
	tzcnt ebp, edi
	tzcnt rbp, qword [0xb64]
	tzcnt rcx, qword [0xf55]
	tzcnt r12w, r13w
	tzcnt r15d, r15d
	tzcnt r14, r13
	tzcnt r24w, r18w
	tzcnt r20d, r21d
	tzcnt r23, r25
	tzcnt cx, word [eax+1]
	tzcnt bx, word [eax+64]
	tzcnt esi, dword [eax+1]
	tzcnt ebx, dword [eax+64]
	tzcnt rbp, qword [eax+1]
	tzcnt rdi, qword [eax+64]
	tzcnt cx, [0xc73]
	tzcnt edi, [0xc95]
	tzcnt rax, [0xdb1]
