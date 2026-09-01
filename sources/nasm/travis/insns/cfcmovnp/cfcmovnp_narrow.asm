	cfcmovnp dx, di
	cfcmovnp si, si
	cfcmovnp ebx, edx
	cfcmovnp edx, ebx
	cfcmovnp ax, dx
	cfcmovnp bp, bx

%ifdef ERROR
	cfcmovnp rdi, rcx
	cfcmovnp rdi, rsi
	cfcmovnp r9w, r15w
	cfcmovnp r10d, r10d
	cfcmovnp r13, r9
	cfcmovnp r14w, r11w
	cfcmovnp r21w, r20w
	cfcmovnp r22d, r27d
	cfcmovnp r22, r22
	cfcmovnp r31w, r28w
	cfcmovnp qword [eax+1], rsi
	cfcmovnp qword [eax+64], rdi
	cfcmovnp rsi, qword [eax+1]
	cfcmovnp rdi, qword [eax+64]
	cfcmovnp rbp, rcx, qword [eax+1]
	cfcmovnp rsi, rcx, qword [eax+64]
	cfcmovnp [0xfce], rcx
	cfcmovnp rbx, [0x374]
	cfcmovnp rsi, rdi, [0xb4b]
%endif
