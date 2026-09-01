default rel
	cmovs dx, bp
	cmovs si, word [0xf6e]
	cmovs esi, eax
	cmovs ebp, dword [0x150]
	cmovs rdi, qword [0xb71]
	cmovs rdi, qword [0xd22]
	cmovs si, cx, si
	cmovs ax, bp, bp
	cmovs r13w, r9w
	cmovs r12d, r14d
	cmovs r12, r8
	cmovs r10w, r10w, r11w
	cmovs r26w, r28w
	cmovs r19d, r27d
	cmovs r21, r23
	cmovs r31w, r28w, r26w
	cmovs si, word [eax+1]
	cmovs di, word [eax+64]
	cmovs edx, dword [eax+1]
	cmovs eax, dword [eax+64]
	cmovs rdi, qword [eax+1]
	cmovs rcx, qword [eax+64]
	cmovs bp, di, word [eax+1]
	cmovs dx, cx, word [eax+64]
	cmovs ecx, esi, dword [eax+1]
	cmovs ebx, edx, dword [eax+64]
	cmovs rdi, rbp, qword [eax+1]
	cmovs rax, rcx, qword [eax+64]
	cmovs ax, [0x10a]
	cmovs ebp, [0xec3]
	cmovs rsi, [0x97d]
	cmovs bp, si, [0x770]
	cmovs ecx, edx, [0x21f]
	cmovs rbx, rdi, [0x5a6]
