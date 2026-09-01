	ccmpz 14, bl, bl
	ccmpz 2, al, dl
	ccmpz 10, bp, di
	ccmpz 6, si, ax
	ccmpz 4, ecx, ebp
	ccmpz 1, dword [0x7f1], eax

%ifdef ERROR
	ccmpz 14, rax, rdi
	ccmpz 6, rax, rcx
	ccmpz 9, r10b, r9b
	ccmpz 7, r14w, r13w
	ccmpz 1, r14d, r8d
	ccmpz 0, r13, r15
	ccmpz 0, r24b, r21b
	ccmpz 15, r26w, r26w
	ccmpz 10, r22d, r19d
	ccmpz 12, r31, r27
	ccmpz 13, qword [eax+1], rax
	ccmpz 10, qword [eax+64], rsi
	ccmpz 3, rdx, qword [eax+1]
	ccmpz 0, rax, qword [eax+64]
	ccmpz 14, qword [eax+1], 87
	ccmpz 5, qword [eax+64], 69
	ccmpz 15, qword [eax+1], -233178741
	ccmpz 2, qword [eax+64], -128458952
	ccmpz 7, [0x810], rbp
	ccmpz 2, rax, [0xa6b]
	ccmpz 7, [0x9c1], 23
	ccmpz 10, [0xd57], 433551523
%endif
