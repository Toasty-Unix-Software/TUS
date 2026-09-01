	cmovbe si, dx
	cmovbe dx, word [0x16a]
	cmovbe esi, ecx
	cmovbe edx, dword [0xff6]
	cmovbe dx, di, cx
	cmovbe cx, di, bp

%ifdef ERROR
	cmovbe rdi, rdi
	cmovbe rax, qword [0x8ef]
	cmovbe r14w, r12w
	cmovbe r11d, r12d
	cmovbe r10, r10
	cmovbe r9w, r15w, r14w
	cmovbe r23w, r21w
	cmovbe r18d, r28d
	cmovbe r19, r18
	cmovbe r20w, r28w, r17w
	cmovbe rcx, qword [eax+1]
	cmovbe rbp, qword [eax+64]
	cmovbe rdi, rdi, qword [eax+1]
	cmovbe rax, rdi, qword [eax+64]
	cmovbe rsi, [0x95d]
	cmovbe rdx, rbx, [0x260]
%endif
