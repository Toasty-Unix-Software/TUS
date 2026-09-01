default rel
	cfcmovpo word [0xafe], cx
	cfcmovpo cx, ax
	cfcmovpo ecx, ebx
	cfcmovpo dword [0x5ce], ebp
	cfcmovpo rax, rbp
	cfcmovpo rdi, rax
	cfcmovpo bp, dx
	cfcmovpo bp, si
	cfcmovpo r11w, r15w
	cfcmovpo r11d, r9d
	cfcmovpo r14, r15
	cfcmovpo r12w, r9w
	cfcmovpo r27w, r20w
	cfcmovpo r18d, r17d
	cfcmovpo r31, r25
	cfcmovpo r22w, r23w
	cfcmovpo word [eax+1], bp
	cfcmovpo word [eax+64], si
	cfcmovpo dword [eax+1], edx
	cfcmovpo dword [eax+64], eax
	cfcmovpo qword [eax+1], rax
	cfcmovpo qword [eax+64], rcx
	cfcmovpo bx, word [eax+1]
	cfcmovpo cx, word [eax+64]
	cfcmovpo ecx, dword [eax+1]
	cfcmovpo ecx, dword [eax+64]
	cfcmovpo rbp, qword [eax+1]
	cfcmovpo rbx, qword [eax+64]
	cfcmovpo ax, ax, word [eax+1]
	cfcmovpo si, ax, word [eax+64]
	cfcmovpo eax, eax, dword [eax+1]
	cfcmovpo ecx, ebx, dword [eax+64]
	cfcmovpo rax, rcx, qword [eax+1]
	cfcmovpo rdx, rbp, qword [eax+64]
	cfcmovpo [0x7b5], dx
	cfcmovpo [0x948], ecx
	cfcmovpo [0x584], rsi
	cfcmovpo ax, [0xf72]
	cfcmovpo edx, [0xfb3]
	cfcmovpo rsi, [0x81e]
	cfcmovpo di, cx, [0x791]
	cfcmovpo ebx, ebp, [0xaeb]
	cfcmovpo rbp, rdi, [0x356]
