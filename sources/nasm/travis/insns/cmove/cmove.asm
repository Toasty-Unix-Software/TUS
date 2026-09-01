default rel
	cmove si, dx
	cmove ax, ax
	cmove ebx, ecx
	cmove ebx, esi
	cmove rbp, rdx
	cmove rdi, rbx
	cmove di, si, cx
	cmove di, ax, ax
	cmove r11w, r9w
	cmove r9d, r8d
	cmove r9, r9
	cmove r12w, r13w, r13w
	cmove r24w, r26w
	cmove r30d, r16d
	cmove r20, r31
	cmove r29w, r16w, r17w
	cmove dx, word [eax+1]
	cmove cx, word [eax+64]
	cmove eax, dword [eax+1]
	cmove ebx, dword [eax+64]
	cmove rbp, qword [eax+1]
	cmove rdx, qword [eax+64]
	cmove cx, bx, word [eax+1]
	cmove bx, bx, word [eax+64]
	cmove esi, esi, dword [eax+1]
	cmove edi, ebx, dword [eax+64]
	cmove rbx, rdi, qword [eax+1]
	cmove rdx, rsi, qword [eax+64]
	cmove ax, [0xd1c]
	cmove esi, [0xfca]
	cmove rsi, [0xecc]
	cmove bp, cx, [0xd27]
	cmove ecx, ecx, [0x382]
	cmove rcx, rsi, [0xf31]
