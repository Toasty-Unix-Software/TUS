	shrx ecx, ebp, cl
	shrx ecx, cl
	shrx ebx, dword [0xfb0], dl
	shrx eax, esi, di
	shrx eax, di
	shrx edi, dword [0x621], cx
	shrx ebp, ecx, esi
	shrx ebp, esi
	shrx edi, eax, ebx

%ifdef ERROR
	shrx eax, edx, rdx
	shrx eax, rdx
	shrx edi, ecx, rsi
	shrx r13d, r13d, r9b
	shrx r14d, r13d, r10w
	shrx r8d, r15d, r14d
	shrx r14d, r12d, r8
	shrx r28d, r31d, r20b
	shrx r30d, r27d, r28w
	shrx r23d, r24d, r30d
	shrx r30d, r23d, r21
	shrx eax, dword [eax+1], rbp
	shrx edi, dword [eax+64], rsi
	shrx rbp, qword [eax+1], bl
	shrx rcx, qword [eax+64], bl
	shrx rcx, qword [eax+1], di
	shrx rcx, qword [eax+64], di
	shrx rax, qword [eax+1], eax
	shrx rbx, qword [eax+64], eax
	shrx rbx, qword [eax+1], rdi
	shrx rax, qword [eax+64], rbp
	shrx eax, [0x1c1], rsi
	shrx rdi, [0x390], cl
	shrx rdx, [0xcb8], ax
	shrx rcx, [0x4fc], esi
	shrx rdx, [0x302], rdx
%endif
