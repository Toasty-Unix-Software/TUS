	pext ebx, ebp, edi
	pext ebx, edi
	pext ecx, ecx, eax

%ifdef ERROR
	pext rcx, rdi, rax
	pext rcx, rax
	pext rsi, rbp, qword [0x29c]
	pext r14d, r8d, r8d
	pext r10, r13, r8
	pext r27d, r30d, r24d
	pext r19, r25, r22
	pext rdx, rsi, qword [eax+1]
	pext rdi, rbp, qword [eax+64]
	pext rsi, rbx, [0x4c8]
%endif
