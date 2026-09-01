	cfcmovs bp, dx
	cfcmovs word [0x267], bx
	cfcmovs ebx, ebx
	cfcmovs dword [0x2da], eax
	cfcmovs dx, cx
	cfcmovs cx, si

%ifdef ERROR
	cfcmovs qword [0x6e8], rsi
	cfcmovs rbx, rsi
	cfcmovs r8w, r9w
	cfcmovs r12d, r15d
	cfcmovs r11, r13
	cfcmovs r10w, r13w
	cfcmovs r25w, r26w
	cfcmovs r23d, r25d
	cfcmovs r28, r23
	cfcmovs r20w, r31w
	cfcmovs qword [eax+1], rcx
	cfcmovs qword [eax+64], rax
	cfcmovs rbp, qword [eax+1]
	cfcmovs rsi, qword [eax+64]
	cfcmovs rdx, rax, qword [eax+1]
	cfcmovs rdx, rdi, qword [eax+64]
	cfcmovs [0x74f], rax
	cfcmovs rdx, [0xb9a]
	cfcmovs rdi, rbx, [0xace]
%endif
