	movsxd si, eax
	movsxd si, dword [0xc7c]
	movsxd ebx, ecx
	movsxd ebp, ecx

%ifdef ERROR
	movsxd rax, eax
	movsxd rax, eax
	movsxd rax, ecx
	movsxd rbp, edx
	movsxd r10w, r15d
	movsxd r12d, r9d
	movsxd r8, r10d
	movsxd r17w, r17d
	movsxd r17d, r23d
	movsxd r17, r30d
	movsxd rax, dword [eax+1]
	movsxd rcx, dword [eax+64]
	movsxd rbp, [0x1e4]
%endif
