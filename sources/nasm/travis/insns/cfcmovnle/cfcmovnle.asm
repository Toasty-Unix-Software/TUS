default rel
	cfcmovnle word [0x568], di
	cfcmovnle word [0x56f], bp
	cfcmovnle eax, esi
	cfcmovnle ebx, esi
	cfcmovnle rcx, rdx
	cfcmovnle rcx, rdi
	cfcmovnle bp, si
	cfcmovnle cx, si
	cfcmovnle r9w, r15w
	cfcmovnle r8d, r8d
	cfcmovnle r9, r11
	cfcmovnle r8w, r14w
	cfcmovnle r18w, r23w
	cfcmovnle r22d, r23d
	cfcmovnle r31, r30
	cfcmovnle r26w, r27w
	cfcmovnle word [eax+1], di
	cfcmovnle word [eax+64], bp
	cfcmovnle dword [eax+1], edx
	cfcmovnle dword [eax+64], eax
	cfcmovnle qword [eax+1], rcx
	cfcmovnle qword [eax+64], rsi
	cfcmovnle bx, word [eax+1]
	cfcmovnle dx, word [eax+64]
	cfcmovnle edx, dword [eax+1]
	cfcmovnle esi, dword [eax+64]
	cfcmovnle rdx, qword [eax+1]
	cfcmovnle rbp, qword [eax+64]
	cfcmovnle dx, bx, word [eax+1]
	cfcmovnle bx, cx, word [eax+64]
	cfcmovnle edx, ebp, dword [eax+1]
	cfcmovnle esi, edx, dword [eax+64]
	cfcmovnle rbp, rbp, qword [eax+1]
	cfcmovnle rcx, rsi, qword [eax+64]
	cfcmovnle [0xefa], ax
	cfcmovnle [0x780], ecx
	cfcmovnle [0x289], rsi
	cfcmovnle bp, [0xbf9]
	cfcmovnle ebx, [0x4fb]
	cfcmovnle rbx, [0xa81]
	cfcmovnle bp, di, [0x144]
	cfcmovnle edi, edi, [0x4c8]
	cfcmovnle rax, rax, [0xfcb]
