default rel
	sarx ecx, esi, cl
	sarx ecx, cl
	sarx ebp, dword [0xd24], cl
	sarx esi, ebp, ax
	sarx esi, ax
	sarx edi, esi, dx
	sarx edx, ecx, esi
	sarx edx, esi
	sarx ebp, edi, ebx
	sarx edx, edi, rcx
	sarx edx, rcx
	sarx edx, ebx, rsi
	sarx r9d, r10d, r15b
	sarx r13d, r15d, r8w
	sarx r13d, r11d, r9d
	sarx r12d, r9d, r15
	sarx r30d, r22d, r19b
	sarx r25d, r16d, r26w
	sarx r21d, r25d, r20d
	sarx r23d, r16d, r28
	sarx eax, dword [eax+1], cl
	sarx edi, dword [eax+64], bl
	sarx ebx, dword [eax+1], di
	sarx edx, dword [eax+64], bp
	sarx ebp, dword [eax+1], ecx
	sarx ebx, dword [eax+64], ebp
	sarx ecx, dword [eax+1], rbx
	sarx ebp, dword [eax+64], rbx
	sarx rcx, qword [eax+1], al
	sarx rsi, qword [eax+64], al
	sarx rdx, qword [eax+1], si
	sarx rdi, qword [eax+64], cx
	sarx rdx, qword [eax+1], ebx
	sarx rbx, qword [eax+64], ebp
	sarx rsi, qword [eax+1], rbp
	sarx rdx, qword [eax+64], rdx
	sarx ebx, [0xf7e], dl
	sarx ebp, [0xcaf], bx
	sarx edi, [0xc4b], eax
	sarx ebx, [0xdad], rsi
	sarx rbp, [0xf9f], bl
	sarx rax, [0xa39], cx
	sarx rsi, [0x82c], edi
	sarx rbp, [0x6c0], rdx
