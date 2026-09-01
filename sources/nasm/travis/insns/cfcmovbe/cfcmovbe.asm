default rel
	cfcmovbe bp, bp
	cfcmovbe bp, dx
	cfcmovbe dword [0x6f1], ebp
	cfcmovbe dword [0xbfa], edx
	cfcmovbe rdi, rcx
	cfcmovbe rdx, rdi
	cfcmovbe dx, bx
	cfcmovbe si, word [0x3e4]
	cfcmovbe r12w, r9w
	cfcmovbe r9d, r14d
	cfcmovbe r14, r10
	cfcmovbe r15w, r8w
	cfcmovbe r24w, r22w
	cfcmovbe r20d, r23d
	cfcmovbe r27, r23
	cfcmovbe r27w, r21w
	cfcmovbe word [eax+1], cx
	cfcmovbe word [eax+64], bx
	cfcmovbe dword [eax+1], esi
	cfcmovbe dword [eax+64], ebx
	cfcmovbe qword [eax+1], rdi
	cfcmovbe qword [eax+64], rbp
	cfcmovbe si, word [eax+1]
	cfcmovbe ax, word [eax+64]
	cfcmovbe edx, dword [eax+1]
	cfcmovbe ecx, dword [eax+64]
	cfcmovbe rax, qword [eax+1]
	cfcmovbe rax, qword [eax+64]
	cfcmovbe ax, dx, word [eax+1]
	cfcmovbe cx, ax, word [eax+64]
	cfcmovbe ebp, ebx, dword [eax+1]
	cfcmovbe edi, ebp, dword [eax+64]
	cfcmovbe rbx, rdi, qword [eax+1]
	cfcmovbe rdi, rdi, qword [eax+64]
	cfcmovbe [0x88b], si
	cfcmovbe [0xc76], eax
	cfcmovbe [0xcb2], rbp
	cfcmovbe ax, [0x314]
	cfcmovbe edx, [0xe8f]
	cfcmovbe rsi, [0xfb4]
	cfcmovbe dx, bp, [0x732]
	cfcmovbe ebx, ebp, [0x812]
	cfcmovbe rax, rax, [0xf01]
