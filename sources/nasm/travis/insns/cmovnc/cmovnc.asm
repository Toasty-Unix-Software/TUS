default rel
	cmovnc di, bp
	cmovnc ax, bp
	cmovnc ebx, dword [0xfa7]
	cmovnc ebp, edi
	cmovnc rcx, rcx
	cmovnc rsi, rdi
	cmovnc ax, bp, dx
	cmovnc di, cx, di
	cmovnc r13w, r14w
	cmovnc r13d, r8d
	cmovnc r9, r10
	cmovnc r12w, r12w, r13w
	cmovnc r20w, r21w
	cmovnc r31d, r31d
	cmovnc r30, r27
	cmovnc r19w, r16w, r27w
	cmovnc bp, word [eax+1]
	cmovnc cx, word [eax+64]
	cmovnc esi, dword [eax+1]
	cmovnc ecx, dword [eax+64]
	cmovnc rdi, qword [eax+1]
	cmovnc rsi, qword [eax+64]
	cmovnc bp, bp, word [eax+1]
	cmovnc bx, bp, word [eax+64]
	cmovnc edx, eax, dword [eax+1]
	cmovnc eax, edi, dword [eax+64]
	cmovnc rdi, rax, qword [eax+1]
	cmovnc rsi, rdi, qword [eax+64]
	cmovnc bx, [0xfda]
	cmovnc edi, [0x30b]
	cmovnc rbx, [0x776]
	cmovnc bp, cx, [0x3ce]
	cmovnc edi, ecx, [0x442]
	cmovnc rbp, rdx, [0xbfd]
