	ctestnb 14, dl, bl
	ctestnb 14, byte [0x103], al
	ctestnb 13, word [0xed5], cx
	ctestnb 15, dx, ax
	ctestnb 13, dword [0x170], edx
	ctestnb 1, ecx, eax

%ifdef ERROR
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
	ctestnb 10, qword [eax+1], rax
	ctestnb 0, qword [eax+64], rdi
	ctestnb 7, qword [eax+1], -508263282
	ctestnb 13, qword [eax+64], 181858929
	ctestnb 1, [0x115], rbp
	ctestnb 5, [0xb4f], -309195167
%endif
