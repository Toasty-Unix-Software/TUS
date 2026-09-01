	cfcmovp bp, ax
	cfcmovp word [0x956], dx
	cfcmovp ecx, edx
	cfcmovp dword [0xcb4], esi
	cfcmovp bx, ax
	cfcmovp bx, word [0x677]

%ifdef ERROR
	cfcmovp qword [0x3d4], rax
	cfcmovp rdx, rdx
	cfcmovp r15w, r11w
	cfcmovp r13d, r11d
	cfcmovp r14, r14
	cfcmovp r11w, r13w
	cfcmovp r30w, r26w
	cfcmovp r28d, r19d
	cfcmovp r20, r28
	cfcmovp r28w, r28w
	cfcmovp qword [eax+1], rdi
	cfcmovp qword [eax+64], rdi
	cfcmovp rax, qword [eax+1]
	cfcmovp rdi, qword [eax+64]
	cfcmovp rbx, rax, qword [eax+1]
	cfcmovp rdx, rbx, qword [eax+64]
	cfcmovp [0x3da], rbp
	cfcmovp rdx, [0x1fe]
	cfcmovp rsi, rdi, [0x7ff]
%endif
