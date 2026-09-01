	bzhi edx, eax, ecx
	bzhi edx, ecx
	bzhi ecx, ebp, edx

%ifdef ERROR
	bzhi rax, qword [0x49a], rdx
	bzhi rax, rdx
	bzhi rbx, rsi, rsi
	bzhi r8d, r12d, r9d
	bzhi r8, r10, r10
	bzhi r17d, r17d, r19d
	bzhi r27, r27, r24
	bzhi rax, qword [eax+1], rax
	bzhi rbx, qword [eax+64], rcx
	bzhi rcx, [0x150], rdi
%endif
