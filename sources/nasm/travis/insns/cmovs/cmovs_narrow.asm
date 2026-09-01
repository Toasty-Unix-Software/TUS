	cmovs dx, bp
	cmovs si, word [0xf6e]
	cmovs esi, eax
	cmovs ebp, dword [0x150]
	cmovs si, cx, si
	cmovs ax, bp, bp

%ifdef ERROR
	cmovs rdi, qword [0xb71]
	cmovs rdi, qword [0xd22]
	cmovs r13w, r9w
	cmovs r12d, r14d
	cmovs r12, r8
	cmovs r10w, r10w, r11w
	cmovs r26w, r28w
	cmovs r19d, r27d
	cmovs r21, r23
	cmovs r31w, r28w, r26w
	cmovs rdi, qword [eax+1]
	cmovs rcx, qword [eax+64]
	cmovs rdi, rbp, qword [eax+1]
	cmovs rax, rcx, qword [eax+64]
	cmovs rsi, [0x97d]
	cmovs rbx, rdi, [0x5a6]
%endif
