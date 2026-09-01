	btc ax, bx
	btc cx, bx
	btc edx, ebx
	btc dword [0x790], ebp
	btc si, 0x78
	btc word [0xdc6], 0x99

%ifdef ERROR
	btc rdx, rbp
	btc rax, rdi
	btc r12w, r12w
	btc r14d, r12d
	btc r14, r13
	btc r9w, 0x9f
	btc r24w, r29w
	btc r20d, r16d
	btc r17, r22
	btc r29w, 0x66
	btc qword [eax+1], rbx
	btc qword [eax+64], rbp
	btc [0x6ff], rcx
%endif
