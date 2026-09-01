	cfcmovns word [0xb0a], di
	cfcmovns ax, bp
	cfcmovns ebx, ecx
	cfcmovns edi, edx
	cfcmovns cx, word [0x62d]
	cfcmovns cx, bp

%ifdef ERROR
	cfcmovns rcx, rdi
	cfcmovns qword [0xcdf], rbp
	cfcmovns r15w, r10w
	cfcmovns r11d, r14d
	cfcmovns r15, r13
	cfcmovns r13w, r12w
	cfcmovns r25w, r17w
	cfcmovns r19d, r22d
	cfcmovns r27, r21
	cfcmovns r19w, r24w
	cfcmovns qword [eax+1], rdx
	cfcmovns qword [eax+64], rcx
	cfcmovns rax, qword [eax+1]
	cfcmovns rsi, qword [eax+64]
	cfcmovns rdi, rbp, qword [eax+1]
	cfcmovns rdi, rcx, qword [eax+64]
	cfcmovns [0xe06], rbx
	cfcmovns rbp, [0x379]
	cfcmovns rdx, rdi, [0xfd5]
%endif
