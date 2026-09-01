default rel
	cfcmovs bp, dx
	cfcmovs word [0x267], bx
	cfcmovs ebx, ebx
	cfcmovs dword [0x2da], eax
	cfcmovs qword [0x6e8], rsi
	cfcmovs rbx, rsi
	cfcmovs dx, cx
	cfcmovs cx, si
	cfcmovs r8w, r9w
	cfcmovs r12d, r15d
	cfcmovs r11, r13
	cfcmovs r10w, r13w
	cfcmovs r25w, r26w
	cfcmovs r23d, r25d
	cfcmovs r28, r23
	cfcmovs r20w, r31w
	cfcmovs word [eax+1], ax
	cfcmovs word [eax+64], di
	cfcmovs dword [eax+1], ecx
	cfcmovs dword [eax+64], esi
	cfcmovs qword [eax+1], rcx
	cfcmovs qword [eax+64], rax
	cfcmovs cx, word [eax+1]
	cfcmovs bp, word [eax+64]
	cfcmovs edi, dword [eax+1]
	cfcmovs ecx, dword [eax+64]
	cfcmovs rbp, qword [eax+1]
	cfcmovs rsi, qword [eax+64]
	cfcmovs bx, dx, word [eax+1]
	cfcmovs bx, bx, word [eax+64]
	cfcmovs edx, esi, dword [eax+1]
	cfcmovs eax, ebp, dword [eax+64]
	cfcmovs rdx, rax, qword [eax+1]
	cfcmovs rdx, rdi, qword [eax+64]
	cfcmovs [0xcf2], cx
	cfcmovs [0xb5a], esi
	cfcmovs [0x74f], rax
	cfcmovs bp, [0x7d3]
	cfcmovs esi, [0xcc5]
	cfcmovs rdx, [0xb9a]
	cfcmovs si, ax, [0xef1]
	cfcmovs edx, edx, [0xe19]
	cfcmovs rdi, rbx, [0xace]
