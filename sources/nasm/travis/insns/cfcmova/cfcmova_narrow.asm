	cfcmova cx, dx
	cfcmova bx, ax
	cfcmova eax, esi
	cfcmova dword [0xeaa], edx
	cfcmova ax, bp
	cfcmova si, di

%ifdef ERROR
	cfcmova rdx, rax
	cfcmova qword [0x7e9], rdi
	cfcmova r14w, r8w
	cfcmova r11d, r10d
	cfcmova r12, r9
	cfcmova r15w, r8w
	cfcmova r16w, r22w
	cfcmova r19d, r19d
	cfcmova r29, r27
	cfcmova r19w, r22w
	cfcmova qword [eax+1], rcx
	cfcmova qword [eax+64], rbx
	cfcmova rdi, qword [eax+1]
	cfcmova rdx, qword [eax+64]
	cfcmova rcx, rcx, qword [eax+1]
	cfcmova rax, rcx, qword [eax+64]
	cfcmova [0x9ca], rbp
	cfcmova rcx, [0xc9b]
	cfcmova rdi, rbp, [0x879]
%endif
