default rel
	cfcmovo ax, bx
	cfcmovo word [0xf69], si
	cfcmovo ebp, ebx
	cfcmovo edi, ecx
	cfcmovo rcx, rdx
	cfcmovo qword [0x5c3], rdi
	cfcmovo cx, bx
	cfcmovo bp, dx
	cfcmovo r14w, r14w
	cfcmovo r11d, r8d
	cfcmovo r8, r12
	cfcmovo r9w, r8w
	cfcmovo r27w, r25w
	cfcmovo r19d, r27d
	cfcmovo r21, r29
	cfcmovo r26w, r21w
	cfcmovo word [eax+1], ax
	cfcmovo word [eax+64], dx
	cfcmovo dword [eax+1], esi
	cfcmovo dword [eax+64], edi
	cfcmovo qword [eax+1], rcx
	cfcmovo qword [eax+64], rbp
	cfcmovo bx, word [eax+1]
	cfcmovo si, word [eax+64]
	cfcmovo ebp, dword [eax+1]
	cfcmovo ebp, dword [eax+64]
	cfcmovo rcx, qword [eax+1]
	cfcmovo rdx, qword [eax+64]
	cfcmovo cx, dx, word [eax+1]
	cfcmovo cx, cx, word [eax+64]
	cfcmovo ebx, eax, dword [eax+1]
	cfcmovo ebx, ecx, dword [eax+64]
	cfcmovo rdx, rsi, qword [eax+1]
	cfcmovo rdx, rbx, qword [eax+64]
	cfcmovo [0xe9b], dx
	cfcmovo [0xa68], edi
	cfcmovo [0xe3d], rbp
	cfcmovo dx, [0xa16]
	cfcmovo edx, [0x618]
	cfcmovo rsi, [0x283]
	cfcmovo si, cx, [0xc53]
	cfcmovo edx, eax, [0x79d]
	cfcmovo rbx, rsi, [0x968]
