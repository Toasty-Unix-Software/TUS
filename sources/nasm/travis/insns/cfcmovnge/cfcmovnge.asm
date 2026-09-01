default rel
	cfcmovnge bx, si
	cfcmovnge word [0x908], bp
	cfcmovnge dword [0xcec], ebp
	cfcmovnge dword [0x78f], esi
	cfcmovnge qword [0xd21], rdi
	cfcmovnge qword [0x872], rdx
	cfcmovnge di, si
	cfcmovnge di, si
	cfcmovnge r9w, r12w
	cfcmovnge r10d, r11d
	cfcmovnge r10, r13
	cfcmovnge r12w, r15w
	cfcmovnge r26w, r18w
	cfcmovnge r19d, r25d
	cfcmovnge r17, r29
	cfcmovnge r29w, r22w
	cfcmovnge word [eax+1], cx
	cfcmovnge word [eax+64], ax
	cfcmovnge dword [eax+1], ebp
	cfcmovnge dword [eax+64], ebp
	cfcmovnge qword [eax+1], rbx
	cfcmovnge qword [eax+64], rbx
	cfcmovnge ax, word [eax+1]
	cfcmovnge dx, word [eax+64]
	cfcmovnge ebx, dword [eax+1]
	cfcmovnge esi, dword [eax+64]
	cfcmovnge rbx, qword [eax+1]
	cfcmovnge rdi, qword [eax+64]
	cfcmovnge si, bp, word [eax+1]
	cfcmovnge dx, dx, word [eax+64]
	cfcmovnge ecx, eax, dword [eax+1]
	cfcmovnge eax, ecx, dword [eax+64]
	cfcmovnge rdi, rcx, qword [eax+1]
	cfcmovnge rbx, rbp, qword [eax+64]
	cfcmovnge [0x7d6], si
	cfcmovnge [0x311], ebp
	cfcmovnge [0x205], rbx
	cfcmovnge bx, [0xf9b]
	cfcmovnge eax, [0x618]
	cfcmovnge rdx, [0xfb2]
	cfcmovnge si, si, [0x6b9]
	cfcmovnge esi, edx, [0x399]
	cfcmovnge rdx, rbx, [0xe26]
