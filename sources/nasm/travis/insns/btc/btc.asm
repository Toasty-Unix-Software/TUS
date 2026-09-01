default rel
	btc ax, bx
	btc cx, bx
	btc edx, ebx
	btc dword [0x790], ebp
	btc rdx, rbp
	btc rax, rdi
	btc si, 0x78
	btc word [0xdc6], 0x99
	btc r12w, r12w
	btc r14d, r12d
	btc r14, r13
	btc r9w, 0x9f
	btc r24w, r29w
	btc r20d, r16d
	btc r17, r22
	btc r29w, 0x66
	btc word [eax+1], bx
	btc word [eax+64], cx
	btc dword [eax+1], edx
	btc dword [eax+64], ecx
	btc qword [eax+1], rbx
	btc qword [eax+64], rbp
	btc word [eax+1], 0xbd
	btc word [eax+64], 0x1b
	btc dword [eax+1], 0xde
	btc dword [eax+64], 0xe4
	btc qword [eax+1], 0x81
	btc qword [eax+64], 0x6
	btc [0x6ba], di
	btc [0x3a1], ebx
	btc [0x6ff], rcx
	btc [0x8df], 0x24
	btc [0xb28], 0x23
	btc [0x81d], 0x8b
