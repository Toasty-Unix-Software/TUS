default rel
	cfcmovnbe word [0x101], si
	cfcmovnbe word [0x7e3], ax
	cfcmovnbe esi, ecx
	cfcmovnbe ebx, edi
	cfcmovnbe rax, rbx
	cfcmovnbe rbx, rdi
	cfcmovnbe bp, word [0x8d9]
	cfcmovnbe cx, di
	cfcmovnbe r8w, r9w
	cfcmovnbe r15d, r14d
	cfcmovnbe r15, r8
	cfcmovnbe r13w, r14w
	cfcmovnbe r25w, r31w
	cfcmovnbe r24d, r24d
	cfcmovnbe r16, r29
	cfcmovnbe r28w, r23w
	cfcmovnbe word [eax+1], di
	cfcmovnbe word [eax+64], cx
	cfcmovnbe dword [eax+1], ebx
	cfcmovnbe dword [eax+64], esi
	cfcmovnbe qword [eax+1], rcx
	cfcmovnbe qword [eax+64], rbx
	cfcmovnbe di, word [eax+1]
	cfcmovnbe bp, word [eax+64]
	cfcmovnbe ebp, dword [eax+1]
	cfcmovnbe ebp, dword [eax+64]
	cfcmovnbe rax, qword [eax+1]
	cfcmovnbe rcx, qword [eax+64]
	cfcmovnbe dx, bp, word [eax+1]
	cfcmovnbe dx, bx, word [eax+64]
	cfcmovnbe ebp, edx, dword [eax+1]
	cfcmovnbe edx, eax, dword [eax+64]
	cfcmovnbe rdi, rdx, qword [eax+1]
	cfcmovnbe rdx, rdx, qword [eax+64]
	cfcmovnbe [0x5a0], dx
	cfcmovnbe [0x264], eax
	cfcmovnbe [0x4dc], rdi
	cfcmovnbe di, [0xd8d]
	cfcmovnbe edx, [0xf4c]
	cfcmovnbe rdx, [0x398]
	cfcmovnbe cx, bx, [0x214]
	cfcmovnbe edx, eax, [0x4e2]
	cfcmovnbe rbp, rdi, [0xb5e]
