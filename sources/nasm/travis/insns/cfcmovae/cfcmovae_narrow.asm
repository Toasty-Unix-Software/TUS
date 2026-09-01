	cfcmovae word [0x293], si
	cfcmovae di, ax
	cfcmovae dword [0x99c], ebp
	cfcmovae ebp, ebx
	cfcmovae bp, dx
	cfcmovae dx, word [0xad1]

%ifdef ERROR
	cfcmovae rdi, rdi
	cfcmovae rbp, rdx
	cfcmovae r13w, r10w
	cfcmovae r12d, r10d
	cfcmovae r13, r15
	cfcmovae r8w, r11w
	cfcmovae r16w, r25w
	cfcmovae r26d, r18d
	cfcmovae r19, r31
	cfcmovae r28w, r26w
	cfcmovae qword [eax+1], rdx
	cfcmovae qword [eax+64], rsi
	cfcmovae rbp, qword [eax+1]
	cfcmovae rbx, qword [eax+64]
	cfcmovae rbx, rdi, qword [eax+1]
	cfcmovae rbx, rax, qword [eax+64]
	cfcmovae [0xf76], rdx
	cfcmovae rax, [0x94e]
	cfcmovae rdi, rax, [0xc95]
%endif
