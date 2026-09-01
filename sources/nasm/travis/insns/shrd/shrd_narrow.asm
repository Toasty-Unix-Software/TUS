	shrd cx, si, 0x5b
	shrd word [0x531], di, 0xde
	shrd edx, edx, 0xa2
	shrd dword [0xcda], esi, 0xae
	shrd cx, si, cl
	shrd si, ax, cl

%ifdef ERROR
	shrd rcx, rdx, 0xf9
	shrd qword [0xd3f], rdi, 0x36
	shrd r8w, r10w, 0xf7
	shrd r14d, r9d, 0x4
	shrd r12, r13, 0x4
	shrd r14w, r15w, cl
	shrd r19w, r16w, 0x5
	shrd r22d, r26d, 0xd1
	shrd r17, r28, 0xfb
	shrd r16w, r17w, cl
	shrd qword [eax+1], rax, 0x18
	shrd qword [eax+64], rbp, 0x7e
	shrd qword [eax+1], rsi, cl
	shrd qword [eax+64], rbx, cl
	shrd rbx, qword [eax+1], rcx, 0x84
	shrd rbx, qword [eax+64], rdx, 0x5e
	shrd rbx, qword [eax+1], rsi, cl
	shrd rax, qword [eax+64], rbx, cl
	shrd [0x9bc], rdx, 0xda
	shrd [0xe1a], rdi, cl
	shrd rdi, [0xfce], rcx, 0xf8
	shrd rdi, [0x7f7], rbx, cl
%endif
