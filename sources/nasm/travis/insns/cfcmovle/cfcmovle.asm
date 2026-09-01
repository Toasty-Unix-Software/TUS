default rel
	cfcmovle cx, ax
	cfcmovle cx, si
	cfcmovle esi, esi
	cfcmovle edx, ebp
	cfcmovle rax, rcx
	cfcmovle qword [0xf8f], rbx
	cfcmovle dx, di
	cfcmovle bp, bp
	cfcmovle r15w, r12w
	cfcmovle r13d, r14d
	cfcmovle r15, r14
	cfcmovle r8w, r15w
	cfcmovle r20w, r31w
	cfcmovle r24d, r20d
	cfcmovle r23, r24
	cfcmovle r19w, r16w
	cfcmovle word [eax+1], bx
	cfcmovle word [eax+64], di
	cfcmovle dword [eax+1], ecx
	cfcmovle dword [eax+64], edx
	cfcmovle qword [eax+1], rcx
	cfcmovle qword [eax+64], rax
	cfcmovle dx, word [eax+1]
	cfcmovle ax, word [eax+64]
	cfcmovle ecx, dword [eax+1]
	cfcmovle ebx, dword [eax+64]
	cfcmovle rcx, qword [eax+1]
	cfcmovle rbx, qword [eax+64]
	cfcmovle ax, cx, word [eax+1]
	cfcmovle bx, cx, word [eax+64]
	cfcmovle edi, ebp, dword [eax+1]
	cfcmovle ebx, ebx, dword [eax+64]
	cfcmovle rdi, rsi, qword [eax+1]
	cfcmovle rax, rsi, qword [eax+64]
	cfcmovle [0x457], bp
	cfcmovle [0xfbe], ecx
	cfcmovle [0xc41], rbp
	cfcmovle ax, [0xf14]
	cfcmovle eax, [0x1e8]
	cfcmovle rsi, [0xd64]
	cfcmovle di, cx, [0x59f]
	cfcmovle edi, eax, [0x2ae]
	cfcmovle rdi, rbp, [0x3b3]
