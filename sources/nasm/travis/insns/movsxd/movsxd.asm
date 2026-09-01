default rel
	movsxd rax, eax
	movsxd rax, eax
	movsxd si, eax
	movsxd si, dword [0xc7c]
	movsxd ebx, ecx
	movsxd ebp, ecx
	movsxd rax, ecx
	movsxd rbp, edx
	movsxd r10w, r15d
	movsxd r12d, r9d
	movsxd r8, r10d
	movsxd r17w, r17d
	movsxd r17d, r23d
	movsxd r17, r30d
	movsxd bx, dword [eax+1]
	movsxd bp, dword [eax+64]
	movsxd eax, dword [eax+1]
	movsxd eax, dword [eax+64]
	movsxd rax, dword [eax+1]
	movsxd rcx, dword [eax+64]
	movsxd ax, [0xc52]
	movsxd eax, [0xa76]
	movsxd rbp, [0x1e4]
