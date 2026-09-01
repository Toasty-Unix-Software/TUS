default rel
	cfcmovnp dx, di
	cfcmovnp si, si
	cfcmovnp ebx, edx
	cfcmovnp edx, ebx
	cfcmovnp rdi, rcx
	cfcmovnp rdi, rsi
	cfcmovnp ax, dx
	cfcmovnp bp, bx
	cfcmovnp r9w, r15w
	cfcmovnp r10d, r10d
	cfcmovnp r13, r9
	cfcmovnp r14w, r11w
	cfcmovnp r21w, r20w
	cfcmovnp r22d, r27d
	cfcmovnp r22, r22
	cfcmovnp r31w, r28w
	cfcmovnp word [eax+1], si
	cfcmovnp word [eax+64], si
	cfcmovnp dword [eax+1], ecx
	cfcmovnp dword [eax+64], edi
	cfcmovnp qword [eax+1], rsi
	cfcmovnp qword [eax+64], rdi
	cfcmovnp ax, word [eax+1]
	cfcmovnp dx, word [eax+64]
	cfcmovnp edi, dword [eax+1]
	cfcmovnp ebp, dword [eax+64]
	cfcmovnp rsi, qword [eax+1]
	cfcmovnp rdi, qword [eax+64]
	cfcmovnp di, ax, word [eax+1]
	cfcmovnp ax, bp, word [eax+64]
	cfcmovnp esi, ebp, dword [eax+1]
	cfcmovnp edx, ecx, dword [eax+64]
	cfcmovnp rbp, rcx, qword [eax+1]
	cfcmovnp rsi, rcx, qword [eax+64]
	cfcmovnp [0xb43], di
	cfcmovnp [0x3f7], ecx
	cfcmovnp [0xfce], rcx
	cfcmovnp bp, [0xa59]
	cfcmovnp eax, [0xf47]
	cfcmovnp rbx, [0x374]
	cfcmovnp ax, bx, [0xf6b]
	cfcmovnp edi, ebp, [0x6fb]
	cfcmovnp rsi, rdi, [0xb4b]
