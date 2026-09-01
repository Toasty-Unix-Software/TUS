default rel
	ctestnb 14, dl, bl
	ctestnb 14, byte [0x103], al
	ctestnb 13, word [0xed5], cx
	ctestnb 15, dx, ax
	ctestnb 13, dword [0x170], edx
	ctestnb 1, ecx, eax
	ctestnb 2, rcx, rcx
	ctestnb 12, rsi, rdi
	ctestnb 12, r12b, r15b
	ctestnb 3, r14w, r10w
	ctestnb 1, r12d, r14d
	ctestnb 10, r13, r15
	ctestnb 11, r16b, r31b
	ctestnb 0, r21w, r27w
	ctestnb 8, r26d, r16d
	ctestnb 0, r21, r30
	ctestnb 13, byte [eax+1], dl
	ctestnb 0, byte [eax+64], al
	ctestnb 5, word [eax+1], di
	ctestnb 0, word [eax+64], di
	ctestnb 3, dword [eax+1], ecx
	ctestnb 14, dword [eax+64], edx
	ctestnb 10, qword [eax+1], rax
	ctestnb 0, qword [eax+64], rdi
	ctestnb 0, byte [eax+1], 0x18
	ctestnb 8, byte [eax+64], 0x34
	ctestnb 10, word [eax+1], 0x9583
	ctestnb 10, word [eax+64], 0x93ba
	ctestnb 13, dword [eax+1], 0x53cb673
	ctestnb 11, dword [eax+64], 0x65d3da3
	ctestnb 7, qword [eax+1], -508263282
	ctestnb 13, qword [eax+64], 181858929
	ctestnb 9, [0x5f2], bl
	ctestnb 8, [0x6f4], dx
	ctestnb 13, [0xf16], edi
	ctestnb 1, [0x115], rbp
	ctestnb 10, [0x348], 0xc0
	ctestnb 6, [0x78f], 0x287a
	ctestnb 3, [0x947], 0x1ade05c4
	ctestnb 5, [0xb4f], -309195167
