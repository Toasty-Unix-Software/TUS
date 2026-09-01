	tzcnt bx, dx
	tzcnt si, word [0x6f9]
	tzcnt ebp, dword [0x679]
	tzcnt ebp, edi

%ifdef ERROR
	tzcnt rbp, qword [0xb64]
	tzcnt rcx, qword [0xf55]
	tzcnt r12w, r13w
	tzcnt r15d, r15d
	tzcnt r14, r13
	tzcnt r24w, r18w
	tzcnt r20d, r21d
	tzcnt r23, r25
	tzcnt rbp, qword [eax+1]
	tzcnt rdi, qword [eax+64]
	tzcnt rax, [0xdb1]
%endif
