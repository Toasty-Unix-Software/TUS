	lwpval ebp, eax, 0x298e34ca
	lwpval ebx, esi, 0x187fe72c

%ifdef ERROR
	lwpval rbp, ebp, 0x3f67abc3
	lwpval rcx, dword [0x8b4], 0x2f239784
	lwpval r9d, r14d, 0x3736c3d5
	lwpval r9, r13d, 0x13c904d
	lwpval rdx, dword [eax+1], 0x19e49139
	lwpval rdi, dword [eax+64], 0x1feaa02e
	lwpval rax, [0xa21], 0x2bec609f
%endif
