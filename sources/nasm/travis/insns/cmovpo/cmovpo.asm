default rel
	cmovpo ax, dx
	cmovpo di, word [0x493]
	cmovpo esi, dword [0x9c3]
	cmovpo ebp, dword [0x87a]
	cmovpo rsi, rcx
	cmovpo rbx, rdi
	cmovpo bx, di, ax
	cmovpo ax, cx, word [0xe52]
	cmovpo r11w, r15w
	cmovpo r13d, r10d
	cmovpo r10, r9
	cmovpo r10w, r11w, r15w
	cmovpo r28w, r21w
	cmovpo r21d, r25d
	cmovpo r26, r23
	cmovpo r21w, r26w, r16w
	cmovpo dx, word [eax+1]
	cmovpo bp, word [eax+64]
	cmovpo edi, dword [eax+1]
	cmovpo ebx, dword [eax+64]
	cmovpo rcx, qword [eax+1]
	cmovpo rax, qword [eax+64]
	cmovpo cx, dx, word [eax+1]
	cmovpo si, bx, word [eax+64]
	cmovpo edx, edi, dword [eax+1]
	cmovpo ecx, eax, dword [eax+64]
	cmovpo rdx, rbx, qword [eax+1]
	cmovpo rbp, rbx, qword [eax+64]
	cmovpo ax, [0xf90]
	cmovpo ecx, [0x1ad]
	cmovpo rbp, [0x40b]
	cmovpo dx, cx, [0x33c]
	cmovpo esi, ebx, [0x9d9]
	cmovpo rcx, rcx, [0x625]
