	cmp byte [0x5e7], bl
	cmp byte [0x790], dl
	cmp di, ax
	cmp bp, di
	cmp esi, edi
	cmp edx, edi

%ifdef ERROR
	cmp rsi, rbx
	cmp rdx, rbp
	cmp r13b, r8b
	cmp r9w, r14w
	cmp r15d, r14d
	cmp r10, r9
	cmp r26b, r31b
	cmp r26w, r25w
	cmp r24d, r19d
	cmp r28, r25
	cmp qword [eax+1], rdx
	cmp qword [eax+64], rcx
	cmp rdx, qword [eax+1]
	cmp rbx, qword [eax+64]
	cmp qword [eax+1], -114
	cmp qword [eax+64], -102
	cmp qword [eax+1], -206666260
	cmp qword [eax+64], 23681036
	cmp [0xd5a], rcx
	cmp rdi, [0x501]
	cmp [0x245], -125
	cmp [0x948], -266310897
%endif
