default rel
	cfcmovnc di, ax
	cfcmovnc ax, di
	cfcmovnc ecx, eax
	cfcmovnc ebp, edi
	cfcmovnc rbx, rcx
	cfcmovnc qword [0xbe5], rdi
	cfcmovnc dx, di
	cfcmovnc bx, ax
	cfcmovnc r12w, r15w
	cfcmovnc r9d, r15d
	cfcmovnc r11, r11
	cfcmovnc r15w, r9w
	cfcmovnc r25w, r17w
	cfcmovnc r18d, r16d
	cfcmovnc r30, r26
	cfcmovnc r18w, r31w
	cfcmovnc word [eax+1], si
	cfcmovnc word [eax+64], bx
	cfcmovnc dword [eax+1], ebp
	cfcmovnc dword [eax+64], edx
	cfcmovnc qword [eax+1], rbx
	cfcmovnc qword [eax+64], rdi
	cfcmovnc ax, word [eax+1]
	cfcmovnc ax, word [eax+64]
	cfcmovnc ebx, dword [eax+1]
	cfcmovnc edi, dword [eax+64]
	cfcmovnc rax, qword [eax+1]
	cfcmovnc rax, qword [eax+64]
	cfcmovnc bp, di, word [eax+1]
	cfcmovnc dx, dx, word [eax+64]
	cfcmovnc edx, edx, dword [eax+1]
	cfcmovnc ebx, esi, dword [eax+64]
	cfcmovnc rsi, rax, qword [eax+1]
	cfcmovnc rdx, rdx, qword [eax+64]
	cfcmovnc [0x609], dx
	cfcmovnc [0x9f3], esi
	cfcmovnc [0x183], rdx
	cfcmovnc si, [0x7e8]
	cfcmovnc edx, [0xab9]
	cfcmovnc rbx, [0x6da]
	cfcmovnc si, ax, [0xb0f]
	cfcmovnc eax, ebp, [0xfa6]
	cfcmovnc rbx, rbx, [0xb63]
