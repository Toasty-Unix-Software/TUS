	shlx edi, edi, cl
	shlx edi, cl
	shlx ebx, ecx, bl
	shlx edi, edx, di
	shlx edi, di
	shlx ebp, eax, cx
	shlx eax, dword [0x9ec], ebp
	shlx eax, ebp
	shlx edi, dword [0x36f], edi

%ifdef ERROR
	shlx edx, esi, rcx
	shlx edx, rcx
	shlx edx, edx, rbp
	shlx r15d, r8d, r13b
	shlx r12d, r9d, r9w
	shlx r10d, r13d, r12d
	shlx r15d, r13d, r14
	shlx r29d, r30d, r23b
	shlx r18d, r20d, r23w
	shlx r18d, r25d, r27d
	shlx r21d, r24d, r19
	shlx esi, dword [eax+1], rdx
	shlx eax, dword [eax+64], rax
	shlx rsi, qword [eax+1], al
	shlx rbp, qword [eax+64], dl
	shlx rax, qword [eax+1], di
	shlx rbx, qword [eax+64], cx
	shlx rdx, qword [eax+1], esi
	shlx rcx, qword [eax+64], edi
	shlx rbp, qword [eax+1], rbp
	shlx rbx, qword [eax+64], rdi
	shlx ebp, [0x284], rsi
	shlx rbp, [0x16f], cl
	shlx rdi, [0x55c], si
	shlx rbx, [0xd00], edi
	shlx rdx, [0xa40], rcx
%endif
