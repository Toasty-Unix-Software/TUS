	cmovnge si, dx
	cmovnge dx, cx
	cmovnge edx, dword [0xd15]
	cmovnge edi, esi
	cmovnge bx, cx, cx
	cmovnge dx, dx, word [0x32c]

%ifdef ERROR
	cmovnge rbx, qword [0x5ef]
	cmovnge rdi, rdx
	cmovnge r13w, r13w
	cmovnge r9d, r14d
	cmovnge r11, r9
	cmovnge r8w, r8w, r8w
	cmovnge r30w, r17w
	cmovnge r30d, r24d
	cmovnge r26, r17
	cmovnge r30w, r30w, r16w
	cmovnge rcx, qword [eax+1]
	cmovnge rcx, qword [eax+64]
	cmovnge rcx, rdi, qword [eax+1]
	cmovnge rcx, rdi, qword [eax+64]
	cmovnge rbp, [0xca0]
	cmovnge rdi, rbx, [0x6cc]
%endif
