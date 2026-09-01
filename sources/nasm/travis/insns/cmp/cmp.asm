default rel
	cmp byte [0x5e7], bl
	cmp byte [0x790], dl
	cmp di, ax
	cmp bp, di
	cmp esi, edi
	cmp edx, edi
	cmp rsi, rbx
	cmp rdx, rbp
	cmp r13b, r8b
	cmp r9w, r14w
	cmp r15d, r14d
	cmp r10, r9
	cmp r26b, r31b
	cmp r26w, r25w
	cmp r24d, r19d
	cmp r28, r25
	cmp byte [eax+1], dl
	cmp byte [eax+64], bl
	cmp word [eax+1], bx
	cmp word [eax+64], ax
	cmp dword [eax+1], eax
	cmp dword [eax+64], edi
	cmp qword [eax+1], rdx
	cmp qword [eax+64], rcx
	cmp al, byte [eax+1]
	cmp bl, byte [eax+64]
	cmp di, word [eax+1]
	cmp dx, word [eax+64]
	cmp ebx, dword [eax+1]
	cmp edi, dword [eax+64]
	cmp rdx, qword [eax+1]
	cmp rbx, qword [eax+64]
	cmp byte [eax+1], 0xef
	cmp byte [eax+64], 0x38
	cmp word [eax+1], 58
	cmp word [eax+64], -26
	cmp word [eax+1], 0x528c
	cmp word [eax+64], 0x6423
	cmp dword [eax+1], -85
	cmp dword [eax+64], -121
	cmp dword [eax+1], 0x4aea856
	cmp dword [eax+64], 0x2525186f
	cmp qword [eax+1], -114
	cmp qword [eax+64], -102
	cmp qword [eax+1], -206666260
	cmp qword [eax+64], 23681036
	cmp [0x2bd], dl
	cmp [0xb66], si
	cmp [0x229], ecx
	cmp [0xd5a], rcx
	cmp dl, [0x594]
	cmp ax, [0x904]
	cmp eax, [0x64b]
	cmp rdi, [0x501]
	cmp [0xbfa], 0xce
	cmp [0xdc5], 115
	cmp [0x4bd], 0xe8f9
	cmp [0xb2a], -121
	cmp [0xca6], 0x33fd480e
	cmp [0x245], -125
	cmp [0x948], -266310897
