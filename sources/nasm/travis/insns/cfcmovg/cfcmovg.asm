default rel
	cfcmovg bp, bp
	cfcmovg word [0x919], ax
	cfcmovg dword [0xfbb], ebx
	cfcmovg eax, ebx
	cfcmovg rdx, rax
	cfcmovg rax, rbp
	cfcmovg di, di
	cfcmovg cx, word [0x286]
	cfcmovg r11w, r15w
	cfcmovg r15d, r8d
	cfcmovg r9, r8
	cfcmovg r8w, r15w
	cfcmovg r27w, r24w
	cfcmovg r31d, r25d
	cfcmovg r29, r31
	cfcmovg r27w, r27w
	cfcmovg word [eax+1], ax
	cfcmovg word [eax+64], bp
	cfcmovg dword [eax+1], edi
	cfcmovg dword [eax+64], esi
	cfcmovg qword [eax+1], rbx
	cfcmovg qword [eax+64], rdi
	cfcmovg cx, word [eax+1]
	cfcmovg bp, word [eax+64]
	cfcmovg ebx, dword [eax+1]
	cfcmovg ecx, dword [eax+64]
	cfcmovg rsi, qword [eax+1]
	cfcmovg rdx, qword [eax+64]
	cfcmovg di, bx, word [eax+1]
	cfcmovg cx, ax, word [eax+64]
	cfcmovg edx, ecx, dword [eax+1]
	cfcmovg ebp, ebx, dword [eax+64]
	cfcmovg rax, rsi, qword [eax+1]
	cfcmovg rsi, rsi, qword [eax+64]
	cfcmovg [0xae0], si
	cfcmovg [0x6c5], esi
	cfcmovg [0xfc8], rsi
	cfcmovg si, [0xf92]
	cfcmovg ecx, [0xde8]
	cfcmovg rbx, [0x951]
	cfcmovg di, cx, [0x665]
	cfcmovg edx, eax, [0x908]
	cfcmovg rsi, rbx, [0x106]
