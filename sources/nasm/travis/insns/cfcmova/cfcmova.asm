default rel
	cfcmova cx, dx
	cfcmova bx, ax
	cfcmova eax, esi
	cfcmova dword [0xeaa], edx
	cfcmova rdx, rax
	cfcmova qword [0x7e9], rdi
	cfcmova ax, bp
	cfcmova si, di
	cfcmova r14w, r8w
	cfcmova r11d, r10d
	cfcmova r12, r9
	cfcmova r15w, r8w
	cfcmova r16w, r22w
	cfcmova r19d, r19d
	cfcmova r29, r27
	cfcmova r19w, r22w
	cfcmova word [eax+1], cx
	cfcmova word [eax+64], si
	cfcmova dword [eax+1], ebx
	cfcmova dword [eax+64], ebp
	cfcmova qword [eax+1], rcx
	cfcmova qword [eax+64], rbx
	cfcmova bx, word [eax+1]
	cfcmova ax, word [eax+64]
	cfcmova ebx, dword [eax+1]
	cfcmova ebp, dword [eax+64]
	cfcmova rdi, qword [eax+1]
	cfcmova rdx, qword [eax+64]
	cfcmova bp, cx, word [eax+1]
	cfcmova si, si, word [eax+64]
	cfcmova edx, esi, dword [eax+1]
	cfcmova ebx, ebp, dword [eax+64]
	cfcmova rcx, rcx, qword [eax+1]
	cfcmova rax, rcx, qword [eax+64]
	cfcmova [0xf8c], di
	cfcmova [0x7ad], esi
	cfcmova [0x9ca], rbp
	cfcmova dx, [0x1fd]
	cfcmova edx, [0xedb]
	cfcmova rcx, [0xc9b]
	cfcmova ax, di, [0xe16]
	cfcmova edi, ebx, [0xfb6]
	cfcmova rdi, rbp, [0x879]
