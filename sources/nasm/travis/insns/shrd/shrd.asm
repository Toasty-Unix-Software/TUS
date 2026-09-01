default rel
	shrd cx, si, 0x5b
	shrd word [0x531], di, 0xde
	shrd edx, edx, 0xa2
	shrd dword [0xcda], esi, 0xae
	shrd rcx, rdx, 0xf9
	shrd qword [0xd3f], rdi, 0x36
	shrd cx, si, cl
	shrd si, ax, cl
	shrd r8w, r10w, 0xf7
	shrd r14d, r9d, 0x4
	shrd r12, r13, 0x4
	shrd r14w, r15w, cl
	shrd r19w, r16w, 0x5
	shrd r22d, r26d, 0xd1
	shrd r17, r28, 0xfb
	shrd r16w, r17w, cl
	shrd word [eax+1], bx, 0x12
	shrd word [eax+64], ax, 0xef
	shrd dword [eax+1], esi, 0xcf
	shrd dword [eax+64], esi, 0xe1
	shrd qword [eax+1], rax, 0x18
	shrd qword [eax+64], rbp, 0x7e
	shrd word [eax+1], bp, cl
	shrd word [eax+64], ax, cl
	shrd dword [eax+1], ebp, cl
	shrd dword [eax+64], ebp, cl
	shrd qword [eax+1], rsi, cl
	shrd qword [eax+64], rbx, cl
	shrd bp, word [eax+1], dx, 0x34
	shrd dx, word [eax+64], cx, 0xc4
	shrd edx, dword [eax+1], ebp, 0xb0
	shrd ebp, dword [eax+64], esi, 0xa2
	shrd rbx, qword [eax+1], rcx, 0x84
	shrd rbx, qword [eax+64], rdx, 0x5e
	shrd cx, word [eax+1], bx, cl
	shrd di, word [eax+64], ax, cl
	shrd ebp, dword [eax+1], esi, cl
	shrd esi, dword [eax+64], edx, cl
	shrd rbx, qword [eax+1], rsi, cl
	shrd rax, qword [eax+64], rbx, cl
	shrd [0xd21], ax, 0xf0
	shrd [0x562], eax, 0xfc
	shrd [0x9bc], rdx, 0xda
	shrd [0x128], si, cl
	shrd [0x870], ebp, cl
	shrd [0xe1a], rdi, cl
	shrd di, [0x7ad], si, 0x1b
	shrd ebx, [0x9ff], ebx, 0xda
	shrd rdi, [0xfce], rcx, 0xf8
	shrd dx, [0xb02], di, cl
	shrd esi, [0x915], ecx, cl
	shrd rdi, [0x7f7], rbx, cl
