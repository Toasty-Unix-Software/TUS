	cfcmovbe bp, bp
	cfcmovbe bp, dx
	cfcmovbe dword [0x6f1], ebp
	cfcmovbe dword [0xbfa], edx
	cfcmovbe dx, bx
	cfcmovbe si, word [0x3e4]

%ifdef ERROR
	cfcmovbe rdi, rcx
	cfcmovbe rdx, rdi
	cfcmovbe r12w, r9w
	cfcmovbe r9d, r14d
	cfcmovbe r14, r10
	cfcmovbe r15w, r8w
	cfcmovbe r24w, r22w
	cfcmovbe r20d, r23d
	cfcmovbe r27, r23
	cfcmovbe r27w, r21w
	cfcmovbe qword [eax+1], rdi
	cfcmovbe qword [eax+64], rbp
	cfcmovbe rax, qword [eax+1]
	cfcmovbe rax, qword [eax+64]
	cfcmovbe rbx, rdi, qword [eax+1]
	cfcmovbe rdi, rdi, qword [eax+64]
	cfcmovbe [0xcb2], rbp
	cfcmovbe rsi, [0xfb4]
	cfcmovbe rax, rax, [0xf01]
%endif
