default rel
	cmovpe dx, ax
	cmovpe bx, bp
	cmovpe ebp, ecx
	cmovpe esi, dword [0xbeb]
	cmovpe rsi, rdi
	cmovpe rdx, rbx
	cmovpe di, si, word [0x144]
	cmovpe bx, ax, word [0x269]
	cmovpe r13w, r8w
	cmovpe r12d, r8d
	cmovpe r12, r8
	cmovpe r10w, r12w, r12w
	cmovpe r30w, r22w
	cmovpe r25d, r16d
	cmovpe r31, r24
	cmovpe r29w, r22w, r18w
	cmovpe bp, word [eax+1]
	cmovpe bp, word [eax+64]
	cmovpe ebx, dword [eax+1]
	cmovpe edx, dword [eax+64]
	cmovpe rdx, qword [eax+1]
	cmovpe rdi, qword [eax+64]
	cmovpe dx, bp, word [eax+1]
	cmovpe bp, cx, word [eax+64]
	cmovpe edi, edx, dword [eax+1]
	cmovpe esi, edx, dword [eax+64]
	cmovpe rcx, rcx, qword [eax+1]
	cmovpe rax, rdi, qword [eax+64]
	cmovpe di, [0xe6e]
	cmovpe ebp, [0x392]
	cmovpe rcx, [0x137]
	cmovpe dx, bp, [0xa1d]
	cmovpe esi, eax, [0xb12]
	cmovpe rdx, rdi, [0x11b]
