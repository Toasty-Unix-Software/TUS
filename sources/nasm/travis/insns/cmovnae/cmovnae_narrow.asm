	cmovnae bp, word [0x56f]
	cmovnae bx, di
	cmovnae ecx, esi
	cmovnae ebx, edi
	cmovnae bx, cx, cx
	cmovnae cx, ax, word [0x226]

%ifdef ERROR
	cmovnae rdi, qword [0x483]
	cmovnae rbx, rbp
	cmovnae r13w, r15w
	cmovnae r8d, r8d
	cmovnae r14, r13
	cmovnae r9w, r15w, r14w
	cmovnae r16w, r24w
	cmovnae r21d, r18d
	cmovnae r21, r22
	cmovnae r23w, r17w, r17w
	cmovnae rcx, qword [eax+1]
	cmovnae rax, qword [eax+64]
	cmovnae rsi, rcx, qword [eax+1]
	cmovnae rcx, rbx, qword [eax+64]
	cmovnae rdi, [0x8c0]
	cmovnae rdx, rax, [0x329]
%endif
