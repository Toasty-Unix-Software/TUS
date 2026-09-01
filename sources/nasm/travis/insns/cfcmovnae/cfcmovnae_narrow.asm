	cfcmovnae word [0x738], si
	cfcmovnae word [0x909], bx
	cfcmovnae esi, ecx
	cfcmovnae esi, esi
	cfcmovnae cx, word [0xa17]
	cfcmovnae dx, word [0x9ae]

%ifdef ERROR
	cfcmovnae rax, rbp
	cfcmovnae rcx, rdi
	cfcmovnae r10w, r15w
	cfcmovnae r14d, r10d
	cfcmovnae r8, r12
	cfcmovnae r14w, r10w
	cfcmovnae r22w, r24w
	cfcmovnae r24d, r29d
	cfcmovnae r19, r17
	cfcmovnae r26w, r29w
	cfcmovnae qword [eax+1], rbx
	cfcmovnae qword [eax+64], rdx
	cfcmovnae rdi, qword [eax+1]
	cfcmovnae rax, qword [eax+64]
	cfcmovnae rbp, rbx, qword [eax+1]
	cfcmovnae rbp, rdx, qword [eax+64]
	cfcmovnae [0xce8], rbp
	cfcmovnae rdi, [0xeda]
	cfcmovnae rbx, rbx, [0x512]
%endif
