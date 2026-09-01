default rel
	cfcmovno word [0xde4], ax
	cfcmovno bp, cx
	cfcmovno ecx, ebx
	cfcmovno edx, ebp
	cfcmovno rdx, rcx
	cfcmovno rsi, rbp
	cfcmovno di, word [0x8dc]
	cfcmovno cx, word [0xed5]
	cfcmovno r14w, r15w
	cfcmovno r15d, r8d
	cfcmovno r10, r14
	cfcmovno r14w, r9w
	cfcmovno r19w, r24w
	cfcmovno r27d, r18d
	cfcmovno r26, r31
	cfcmovno r24w, r25w
	cfcmovno word [eax+1], ax
	cfcmovno word [eax+64], cx
	cfcmovno dword [eax+1], eax
	cfcmovno dword [eax+64], ecx
	cfcmovno qword [eax+1], rbp
	cfcmovno qword [eax+64], rdx
	cfcmovno si, word [eax+1]
	cfcmovno di, word [eax+64]
	cfcmovno esi, dword [eax+1]
	cfcmovno eax, dword [eax+64]
	cfcmovno rdi, qword [eax+1]
	cfcmovno rbx, qword [eax+64]
	cfcmovno di, bp, word [eax+1]
	cfcmovno bp, si, word [eax+64]
	cfcmovno edx, edi, dword [eax+1]
	cfcmovno ebp, eax, dword [eax+64]
	cfcmovno rdx, rdx, qword [eax+1]
	cfcmovno rdx, rsi, qword [eax+64]
	cfcmovno [0xc90], bp
	cfcmovno [0xe73], eax
	cfcmovno [0xd0c], rbx
	cfcmovno bp, [0x563]
	cfcmovno eax, [0xcaf]
	cfcmovno rax, [0x642]
	cfcmovno si, bx, [0x7a2]
	cfcmovno ecx, edi, [0xeed]
	cfcmovno rdi, rbx, [0xf88]
