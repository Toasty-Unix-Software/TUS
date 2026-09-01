default rel
	cfcmovp bp, ax
	cfcmovp word [0x956], dx
	cfcmovp ecx, edx
	cfcmovp dword [0xcb4], esi
	cfcmovp qword [0x3d4], rax
	cfcmovp rdx, rdx
	cfcmovp bx, ax
	cfcmovp bx, word [0x677]
	cfcmovp r15w, r11w
	cfcmovp r13d, r11d
	cfcmovp r14, r14
	cfcmovp r11w, r13w
	cfcmovp r30w, r26w
	cfcmovp r28d, r19d
	cfcmovp r20, r28
	cfcmovp r28w, r28w
	cfcmovp word [eax+1], ax
	cfcmovp word [eax+64], dx
	cfcmovp dword [eax+1], ebp
	cfcmovp dword [eax+64], ebp
	cfcmovp qword [eax+1], rdi
	cfcmovp qword [eax+64], rdi
	cfcmovp bx, word [eax+1]
	cfcmovp bp, word [eax+64]
	cfcmovp edi, dword [eax+1]
	cfcmovp eax, dword [eax+64]
	cfcmovp rax, qword [eax+1]
	cfcmovp rdi, qword [eax+64]
	cfcmovp dx, bx, word [eax+1]
	cfcmovp cx, ax, word [eax+64]
	cfcmovp ebp, ebx, dword [eax+1]
	cfcmovp edx, edi, dword [eax+64]
	cfcmovp rbx, rax, qword [eax+1]
	cfcmovp rdx, rbx, qword [eax+64]
	cfcmovp [0xb26], di
	cfcmovp [0xdd0], ebp
	cfcmovp [0x3da], rbp
	cfcmovp dx, [0x9ab]
	cfcmovp ebx, [0xb61]
	cfcmovp rdx, [0x1fe]
	cfcmovp dx, si, [0x34b]
	cfcmovp edi, eax, [0xa7e]
	cfcmovp rsi, rdi, [0x7ff]
