	cmovnc di, bp
	cmovnc ax, bp
	cmovnc ebx, dword [0xfa7]
	cmovnc ebp, edi
	cmovnc ax, bp, dx
	cmovnc di, cx, di

%ifdef ERROR
	cmovnc rcx, rcx
	cmovnc rsi, rdi
	cmovnc r13w, r14w
	cmovnc r13d, r8d
	cmovnc r9, r10
	cmovnc r12w, r12w, r13w
	cmovnc r20w, r21w
	cmovnc r31d, r31d
	cmovnc r30, r27
	cmovnc r19w, r16w, r27w
	cmovnc rdi, qword [eax+1]
	cmovnc rsi, qword [eax+64]
	cmovnc rdi, rax, qword [eax+1]
	cmovnc rsi, rdi, qword [eax+64]
	cmovnc rbx, [0x776]
	cmovnc rbp, rdx, [0xbfd]
%endif
