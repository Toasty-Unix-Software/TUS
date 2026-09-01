	ud2a
	ud2a
	ud2a bx, word [0x923]
	ud2a bx, word [0x3e2]
	ud2a eax, esi
	ud2a ebx, ebx

%ifdef ERROR
	ud2a rdx, rbp
	ud2a rcx, rcx
	ud2a r15w, r12w
	ud2a r15d, r14d
	ud2a r13, r8
	ud2a r17w, r20w
	ud2a r16d, r26d
	ud2a r20, r20
	ud2a rbx, qword [eax+1]
	ud2a rdi, qword [eax+64]
	ud2a rbx, [0x6cf]
%endif
