	cmovnp cx, si
	cmovnp dx, bp
	cmovnp ecx, edx
	cmovnp edi, esi
	cmovnp di, bx, bx
	cmovnp cx, bx, si

%ifdef ERROR
	cmovnp rax, rcx
	cmovnp rdi, rdi
	cmovnp r10w, r15w
	cmovnp r14d, r11d
	cmovnp r9, r8
	cmovnp r13w, r14w, r10w
	cmovnp r20w, r31w
	cmovnp r20d, r24d
	cmovnp r26, r24
	cmovnp r21w, r28w, r29w
	cmovnp rbp, qword [eax+1]
	cmovnp rbx, qword [eax+64]
	cmovnp rdi, rax, qword [eax+1]
	cmovnp rbp, rdi, qword [eax+64]
	cmovnp rax, [0x206]
	cmovnp rax, rcx, [0x5e8]
%endif
