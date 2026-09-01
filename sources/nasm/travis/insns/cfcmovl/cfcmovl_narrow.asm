	cfcmovl word [0xe41], bp
	cfcmovl ax, dx
	cfcmovl eax, edi
	cfcmovl eax, ecx
	cfcmovl dx, bx
	cfcmovl si, si

%ifdef ERROR
	cfcmovl rax, rbp
	cfcmovl rax, rdx
	cfcmovl r15w, r14w
	cfcmovl r15d, r10d
	cfcmovl r10, r9
	cfcmovl r9w, r14w
	cfcmovl r22w, r19w
	cfcmovl r19d, r27d
	cfcmovl r26, r24
	cfcmovl r22w, r20w
	cfcmovl qword [eax+1], rdx
	cfcmovl qword [eax+64], rdi
	cfcmovl rax, qword [eax+1]
	cfcmovl rcx, qword [eax+64]
	cfcmovl rdx, rsi, qword [eax+1]
	cfcmovl rax, rdx, qword [eax+64]
	cfcmovl [0x49f], rcx
	cfcmovl rsi, [0x5ef]
	cfcmovl rbp, rax, [0x96c]
%endif
