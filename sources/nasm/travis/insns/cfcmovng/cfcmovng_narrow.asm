	cfcmovng word [0xd33], bp
	cfcmovng cx, bp
	cfcmovng edx, eax
	cfcmovng edx, ebp
	cfcmovng di, bp
	cfcmovng si, bp

%ifdef ERROR
	cfcmovng rcx, rcx
	cfcmovng qword [0xa62], rdx
	cfcmovng r10w, r12w
	cfcmovng r13d, r8d
	cfcmovng r10, r12
	cfcmovng r9w, r12w
	cfcmovng r23w, r27w
	cfcmovng r25d, r17d
	cfcmovng r28, r23
	cfcmovng r18w, r30w
	cfcmovng qword [eax+1], rdx
	cfcmovng qword [eax+64], rdx
	cfcmovng rcx, qword [eax+1]
	cfcmovng rdx, qword [eax+64]
	cfcmovng rsi, rdi, qword [eax+1]
	cfcmovng rdx, rbp, qword [eax+64]
	cfcmovng [0xe40], rcx
	cfcmovng rdi, [0x80b]
	cfcmovng rdi, rax, [0xd64]
%endif
