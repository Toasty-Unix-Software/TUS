	cfcmovnge bx, si
	cfcmovnge word [0x908], bp
	cfcmovnge dword [0xcec], ebp
	cfcmovnge dword [0x78f], esi
	cfcmovnge di, si
	cfcmovnge di, si

%ifdef ERROR
	cfcmovnge qword [0xd21], rdi
	cfcmovnge qword [0x872], rdx
	cfcmovnge r9w, r12w
	cfcmovnge r10d, r11d
	cfcmovnge r10, r13
	cfcmovnge r12w, r15w
	cfcmovnge r26w, r18w
	cfcmovnge r19d, r25d
	cfcmovnge r17, r29
	cfcmovnge r29w, r22w
	cfcmovnge qword [eax+1], rbx
	cfcmovnge qword [eax+64], rbx
	cfcmovnge rbx, qword [eax+1]
	cfcmovnge rdi, qword [eax+64]
	cfcmovnge rdi, rcx, qword [eax+1]
	cfcmovnge rbx, rbp, qword [eax+64]
	cfcmovnge [0x205], rbx
	cfcmovnge rdx, [0xfb2]
	cfcmovnge rdx, rbx, [0xe26]
%endif
