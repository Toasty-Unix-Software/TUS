default rel
	rcl dl, 1
	rcl byte [0xfbe], 1
	rcl cx, 1
	rcl ax, 1
	rcl eax, 1
	rcl dword [0xc4e], 1
	rcl rdx, 1
	rcl rsi, 1
	rcl r15b, 1
	rcl r9w, 1
	rcl r12d, 1
	rcl r10, 1
	rcl r24b, 1
	rcl r29w, 1
	rcl r26d, 1
	rcl r25, 1
	rcl byte [eax+1], 1
	rcl byte [eax+64], 1
	rcl word [eax+1], 1
	rcl word [eax+64], 1
	rcl dword [eax+1], 1
	rcl dword [eax+64], 1
	rcl qword [eax+1], 1
	rcl qword [eax+64], 1
	rcl byte [eax+1], cl
	rcl byte [eax+64], cl
	rcl word [eax+1], cl
	rcl word [eax+64], cl
	rcl dword [eax+1], cl
	rcl dword [eax+64], cl
	rcl qword [eax+1], cl
	rcl qword [eax+64], cl
	rcl byte [eax+1], cx
	rcl byte [eax+64], cx
	rcl word [eax+1], cx
	rcl word [eax+64], cx
	rcl dword [eax+1], cx
	rcl dword [eax+64], cx
	rcl qword [eax+1], cx
	rcl qword [eax+64], cx
	rcl byte [eax+1], ecx
	rcl byte [eax+64], ecx
	rcl word [eax+1], ecx
	rcl word [eax+64], ecx
	rcl dword [eax+1], ecx
	rcl dword [eax+64], ecx
	rcl qword [eax+1], ecx
	rcl qword [eax+64], ecx
	rcl byte [eax+1], 0x9
	rcl byte [eax+64], 0xbc
	rcl word [eax+1], 0x89
	rcl word [eax+64], 0x2e
	rcl dword [eax+1], 0x77
	rcl dword [eax+64], 0xac
	rcl qword [eax+1], 0x82
	rcl qword [eax+64], 0xc7
	rcl cl, byte [eax+1], 1
	rcl al, byte [eax+64], 1
	rcl di, word [eax+1], 1
	rcl di, word [eax+64], 1
	rcl eax, dword [eax+1], 1
	rcl ebx, dword [eax+64], 1
	rcl rbp, qword [eax+1], 1
	rcl rbp, qword [eax+64], 1
	rcl dl, byte [eax+1], cl
	rcl al, byte [eax+64], cl
	rcl bx, word [eax+1], cl
	rcl ax, word [eax+64], cl
	rcl esi, dword [eax+1], cl
	rcl esi, dword [eax+64], cl
	rcl rdx, qword [eax+1], cl
	rcl rbp, qword [eax+64], cl
	rcl bl, byte [eax+1], cx
	rcl cl, byte [eax+64], cx
	rcl cx, word [eax+1], cx
	rcl bp, word [eax+64], cx
	rcl esi, dword [eax+1], cx
	rcl ecx, dword [eax+64], cx
	rcl rsi, qword [eax+1], cx
	rcl rsi, qword [eax+64], cx
	rcl cl, byte [eax+1], ecx
	rcl dl, byte [eax+64], ecx
	rcl bp, word [eax+1], ecx
	rcl bp, word [eax+64], ecx
	rcl ebx, dword [eax+1], ecx
	rcl ecx, dword [eax+64], ecx
	rcl rbp, qword [eax+1], ecx
	rcl rax, qword [eax+64], ecx
	rcl dl, byte [eax+1], 0x78
	rcl dl, byte [eax+64], 0xf3
	rcl si, word [eax+1], 0xdb
	rcl dx, word [eax+64], 0x7a
	rcl edx, dword [eax+1], 0xa8
	rcl edi, dword [eax+64], 0x2c
	rcl rbx, qword [eax+1], 0xda
	rcl rax, qword [eax+64], 0xf6
	rcl [0xb0c], 1
	rcl [0xce0], 1
	rcl [0x8e9], 1
	rcl [0xca6], 1
	rcl [0x13d], cl
	rcl [0x2ed], cl
	rcl [0xf3f], cl
	rcl [0xe06], cl
	rcl [0x964], cx
	rcl [0x528], cx
	rcl [0xe78], cx
	rcl [0xa40], cx
	rcl [0xa1f], ecx
	rcl [0xdcf], ecx
	rcl [0x864], ecx
	rcl [0x5a6], ecx
	rcl [0x39e], 0x2e
	rcl [0x28e], 0xf1
	rcl [0x1b6], 0xc0
	rcl [0x934], 0x88
	rcl cl, [0xa99], 1
	rcl ax, [0x26c], 1
	rcl ecx, [0x6d5], 1
	rcl rdx, [0x456], 1
	rcl bl, [0xf6b], cl
	rcl ax, [0xee8], cl
	rcl edi, [0x3fc], cl
	rcl rdi, [0xbf9], cl
	rcl dl, [0x1e3], cx
	rcl di, [0xfa1], cx
	rcl ebp, [0xd9d], cx
	rcl rax, [0x5cf], cx
	rcl cl, [0x79d], ecx
	rcl ax, [0xf72], ecx
	rcl ecx, [0xb36], ecx
	rcl rdx, [0x17f], ecx
	rcl dl, [0x262], 0xb5
	rcl di, [0x250], 0x31
	rcl ebp, [0x227], 0x66
	rcl rdx, [0xe9a], 0xbd
