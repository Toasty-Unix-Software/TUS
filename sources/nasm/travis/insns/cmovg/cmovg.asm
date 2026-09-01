default rel
	cmovg si, bp
	cmovg bp, si
	cmovg ebx, edi
	cmovg ecx, ecx
	cmovg rax, rsi
	cmovg rax, rax
	cmovg si, si, di
	cmovg si, dx, word [0x8b0]
	cmovg r9w, r10w
	cmovg r9d, r10d
	cmovg r15, r14
	cmovg r10w, r15w, r9w
	cmovg r16w, r17w
	cmovg r23d, r27d
	cmovg r31, r16
	cmovg r17w, r20w, r29w
	cmovg bx, word [eax+1]
	cmovg di, word [eax+64]
	cmovg ecx, dword [eax+1]
	cmovg ebp, dword [eax+64]
	cmovg rbp, qword [eax+1]
	cmovg rcx, qword [eax+64]
	cmovg bp, ax, word [eax+1]
	cmovg bp, di, word [eax+64]
	cmovg ebx, edx, dword [eax+1]
	cmovg edi, esi, dword [eax+64]
	cmovg rax, rcx, qword [eax+1]
	cmovg rcx, rsi, qword [eax+64]
	cmovg bp, [0xb80]
	cmovg ebx, [0x274]
	cmovg rdi, [0x786]
	cmovg bp, dx, [0x11b]
	cmovg esi, edx, [0x5f7]
	cmovg rdi, rdi, [0xb99]
