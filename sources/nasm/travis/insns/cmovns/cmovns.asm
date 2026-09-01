default rel
	cmovns dx, ax
	cmovns cx, word [0x70e]
	cmovns ebx, edi
	cmovns eax, eax
	cmovns rax, qword [0xdf3]
	cmovns rax, qword [0xe17]
	cmovns ax, di, si
	cmovns bx, bp, word [0x989]
	cmovns r11w, r14w
	cmovns r14d, r10d
	cmovns r10, r9
	cmovns r10w, r15w, r14w
	cmovns r21w, r30w
	cmovns r23d, r29d
	cmovns r29, r31
	cmovns r25w, r29w, r24w
	cmovns bp, word [eax+1]
	cmovns di, word [eax+64]
	cmovns ebx, dword [eax+1]
	cmovns ecx, dword [eax+64]
	cmovns rsi, qword [eax+1]
	cmovns rax, qword [eax+64]
	cmovns cx, si, word [eax+1]
	cmovns ax, si, word [eax+64]
	cmovns edx, edx, dword [eax+1]
	cmovns edi, esi, dword [eax+64]
	cmovns rdi, rdi, qword [eax+1]
	cmovns rbx, rax, qword [eax+64]
	cmovns bp, [0xb31]
	cmovns edi, [0x2eb]
	cmovns rax, [0x47b]
	cmovns ax, dx, [0x4e9]
	cmovns edx, eax, [0xb96]
	cmovns rax, rsi, [0xd82]
