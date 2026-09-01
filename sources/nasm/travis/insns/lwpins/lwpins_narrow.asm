	lwpins esi, ebp, 0x1a6c9076
	lwpins ebp, ecx, 0x3614ad80

%ifdef ERROR
	lwpins rcx, esi, 0x3b582d4c
	lwpins rdx, eax, 0x2469711e
	lwpins r8d, r12d, 0x22ae5f5a
	lwpins r12, r10d, 0x132b9602
	lwpins rcx, dword [eax+1], 0x2c131cdf
	lwpins rbp, dword [eax+64], 0x3ea7214f
	lwpins rax, [0x342], 0x2cf6e9b
%endif
