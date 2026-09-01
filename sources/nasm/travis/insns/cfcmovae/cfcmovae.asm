default rel
	cfcmovae word [0x293], si
	cfcmovae di, ax
	cfcmovae dword [0x99c], ebp
	cfcmovae ebp, ebx
	cfcmovae rdi, rdi
	cfcmovae rbp, rdx
	cfcmovae bp, dx
	cfcmovae dx, word [0xad1]
	cfcmovae r13w, r10w
	cfcmovae r12d, r10d
	cfcmovae r13, r15
	cfcmovae r8w, r11w
	cfcmovae r16w, r25w
	cfcmovae r26d, r18d
	cfcmovae r19, r31
	cfcmovae r28w, r26w
	cfcmovae word [eax+1], ax
	cfcmovae word [eax+64], bx
	cfcmovae dword [eax+1], eax
	cfcmovae dword [eax+64], edx
	cfcmovae qword [eax+1], rdx
	cfcmovae qword [eax+64], rsi
	cfcmovae cx, word [eax+1]
	cfcmovae cx, word [eax+64]
	cfcmovae edi, dword [eax+1]
	cfcmovae edx, dword [eax+64]
	cfcmovae rbp, qword [eax+1]
	cfcmovae rbx, qword [eax+64]
	cfcmovae bp, dx, word [eax+1]
	cfcmovae dx, di, word [eax+64]
	cfcmovae edx, ecx, dword [eax+1]
	cfcmovae edi, ebp, dword [eax+64]
	cfcmovae rbx, rdi, qword [eax+1]
	cfcmovae rbx, rax, qword [eax+64]
	cfcmovae [0xc0f], ax
	cfcmovae [0x540], esi
	cfcmovae [0xf76], rdx
	cfcmovae bx, [0x4a7]
	cfcmovae ecx, [0x50d]
	cfcmovae rax, [0x94e]
	cfcmovae di, bp, [0xd3d]
	cfcmovae edi, ecx, [0x11c]
	cfcmovae rdi, rax, [0xc95]
