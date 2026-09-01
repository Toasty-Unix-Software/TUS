default rel
	cfcmovnl si, bp
	cfcmovnl word [0xc66], di
	cfcmovnl eax, esi
	cfcmovnl dword [0x410], edi
	cfcmovnl rax, rbp
	cfcmovnl qword [0x58a], rbp
	cfcmovnl si, word [0x203]
	cfcmovnl bp, ax
	cfcmovnl r12w, r13w
	cfcmovnl r12d, r14d
	cfcmovnl r11, r15
	cfcmovnl r12w, r13w
	cfcmovnl r18w, r28w
	cfcmovnl r26d, r22d
	cfcmovnl r28, r16
	cfcmovnl r17w, r19w
	cfcmovnl word [eax+1], bx
	cfcmovnl word [eax+64], bx
	cfcmovnl dword [eax+1], edx
	cfcmovnl dword [eax+64], ebp
	cfcmovnl qword [eax+1], rdx
	cfcmovnl qword [eax+64], rdx
	cfcmovnl bp, word [eax+1]
	cfcmovnl bx, word [eax+64]
	cfcmovnl esi, dword [eax+1]
	cfcmovnl ebp, dword [eax+64]
	cfcmovnl rdx, qword [eax+1]
	cfcmovnl rcx, qword [eax+64]
	cfcmovnl ax, bp, word [eax+1]
	cfcmovnl ax, dx, word [eax+64]
	cfcmovnl esi, ebx, dword [eax+1]
	cfcmovnl esi, ecx, dword [eax+64]
	cfcmovnl rdx, rcx, qword [eax+1]
	cfcmovnl rbp, rdx, qword [eax+64]
	cfcmovnl [0xfff], si
	cfcmovnl [0xa4d], ebp
	cfcmovnl [0xc92], rdx
	cfcmovnl cx, [0x5f8]
	cfcmovnl eax, [0x3d3]
	cfcmovnl rcx, [0xb6f]
	cfcmovnl cx, cx, [0x309]
	cfcmovnl ebx, ebx, [0xf97]
	cfcmovnl rdi, rbp, [0xedb]
