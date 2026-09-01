	ctestae 6, byte [0x5f6], dl
	ctestae 6, bl, cl
	ctestae 6, bp, di
	ctestae 3, cx, cx
	ctestae 10, edi, ebx
	ctestae 7, ecx, edi

%ifdef ERROR
	ctestae 8, qword [0xd7b], rbp
	ctestae 6, qword [0x27c], rbx
	ctestae 10, r13b, r10b
	ctestae 1, r11w, r12w
	ctestae 4, r15d, r11d
	ctestae 6, r13, r9
	ctestae 12, r26b, r19b
	ctestae 1, r26w, r22w
	ctestae 0, r21d, r24d
	ctestae 6, r20, r19
	ctestae 13, qword [eax+1], rdi
	ctestae 0, qword [eax+64], rbx
	ctestae 10, qword [eax+1], -11623662
	ctestae 11, qword [eax+64], 189146921
	ctestae 15, [0x492], rdx
	ctestae 12, [0x3cc], 287911890
%endif
