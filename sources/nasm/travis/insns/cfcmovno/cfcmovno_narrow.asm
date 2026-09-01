	cfcmovno word [0xde4], ax
	cfcmovno bp, cx
	cfcmovno ecx, ebx
	cfcmovno edx, ebp
	cfcmovno di, word [0x8dc]
	cfcmovno cx, word [0xed5]

%ifdef ERROR
	cfcmovno rdx, rcx
	cfcmovno rsi, rbp
	cfcmovno r14w, r15w
	cfcmovno r15d, r8d
	cfcmovno r10, r14
	cfcmovno r14w, r9w
	cfcmovno r19w, r24w
	cfcmovno r27d, r18d
	cfcmovno r26, r31
	cfcmovno r24w, r25w
	cfcmovno qword [eax+1], rbp
	cfcmovno qword [eax+64], rdx
	cfcmovno rdi, qword [eax+1]
	cfcmovno rbx, qword [eax+64]
	cfcmovno rdx, rdx, qword [eax+1]
	cfcmovno rdx, rsi, qword [eax+64]
	cfcmovno [0xd0c], rbx
	cfcmovno rax, [0x642]
	cfcmovno rdi, rbx, [0xf88]
%endif
