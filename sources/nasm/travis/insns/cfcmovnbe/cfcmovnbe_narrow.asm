	cfcmovnbe word [0x101], si
	cfcmovnbe word [0x7e3], ax
	cfcmovnbe esi, ecx
	cfcmovnbe ebx, edi
	cfcmovnbe bp, word [0x8d9]
	cfcmovnbe cx, di

%ifdef ERROR
	cfcmovnbe rax, rbx
	cfcmovnbe rbx, rdi
	cfcmovnbe r8w, r9w
	cfcmovnbe r15d, r14d
	cfcmovnbe r15, r8
	cfcmovnbe r13w, r14w
	cfcmovnbe r25w, r31w
	cfcmovnbe r24d, r24d
	cfcmovnbe r16, r29
	cfcmovnbe r28w, r23w
	cfcmovnbe qword [eax+1], rcx
	cfcmovnbe qword [eax+64], rbx
	cfcmovnbe rax, qword [eax+1]
	cfcmovnbe rcx, qword [eax+64]
	cfcmovnbe rdi, rdx, qword [eax+1]
	cfcmovnbe rdx, rdx, qword [eax+64]
	cfcmovnbe [0x4dc], rdi
	cfcmovnbe rdx, [0x398]
	cfcmovnbe rbp, rdi, [0xb5e]
%endif
