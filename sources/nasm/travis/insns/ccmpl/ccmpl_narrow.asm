	ccmpl 8, dl, al
	ccmpl 15, bl, dl
	ccmpl 4, si, bx
	ccmpl 15, word [0x50c], bx
	ccmpl 10, dword [0xabe], esi
	ccmpl 10, edx, edx

%ifdef ERROR
	ccmpl 11, rbp, rdx
	ccmpl 1, qword [0xad5], rbp
	ccmpl 5, r13b, r14b
	ccmpl 0, r13w, r15w
	ccmpl 1, r12d, r15d
	ccmpl 3, r9, r14
	ccmpl 11, r17b, r28b
	ccmpl 10, r19w, r17w
	ccmpl 2, r21d, r26d
	ccmpl 5, r25, r28
	ccmpl 6, qword [eax+1], rdx
	ccmpl 6, qword [eax+64], rdi
	ccmpl 5, rdi, qword [eax+1]
	ccmpl 11, rsi, qword [eax+64]
	ccmpl 9, qword [eax+1], 63
	ccmpl 2, qword [eax+64], -36
	ccmpl 11, qword [eax+1], -381212624
	ccmpl 9, qword [eax+64], 500905960
	ccmpl 13, [0xafe], rbx
	ccmpl 8, rax, [0x704]
	ccmpl 5, [0x50a], -31
	ccmpl 9, [0x94b], 514720756
%endif
