default rel
	cmovnp cx, si
	cmovnp dx, bp
	cmovnp ecx, edx
	cmovnp edi, esi
	cmovnp rax, rcx
	cmovnp rdi, rdi
	cmovnp di, bx, bx
	cmovnp cx, bx, si
	cmovnp r10w, r15w
	cmovnp r14d, r11d
	cmovnp r9, r8
	cmovnp r13w, r14w, r10w
	cmovnp r20w, r31w
	cmovnp r20d, r24d
	cmovnp r26, r24
	cmovnp r21w, r28w, r29w
	cmovnp ax, word [eax+1]
	cmovnp si, word [eax+64]
	cmovnp ebp, dword [eax+1]
	cmovnp ebp, dword [eax+64]
	cmovnp rbp, qword [eax+1]
	cmovnp rbx, qword [eax+64]
	cmovnp cx, cx, word [eax+1]
	cmovnp bp, dx, word [eax+64]
	cmovnp ebx, edi, dword [eax+1]
	cmovnp ebx, edx, dword [eax+64]
	cmovnp rdi, rax, qword [eax+1]
	cmovnp rbp, rdi, qword [eax+64]
	cmovnp dx, [0xc77]
	cmovnp eax, [0x18b]
	cmovnp rax, [0x206]
	cmovnp bx, di, [0xf58]
	cmovnp esi, ebx, [0xb32]
	cmovnp rax, rcx, [0x5e8]
