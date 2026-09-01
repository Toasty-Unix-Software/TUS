	ctestle 7, dl, al
	ctestle 4, al, bl
	ctestle 2, bx, di
	ctestle 12, bp, bx
	ctestle 14, ebp, ecx
	ctestle 15, ebx, edx

%ifdef ERROR
	ctestle 11, rbp, rcx
	ctestle 12, rax, rax
	ctestle 10, r14b, r13b
	ctestle 11, r8w, r13w
	ctestle 4, r10d, r11d
	ctestle 6, r13, r15
	ctestle 13, r23b, r18b
	ctestle 5, r19w, r26w
	ctestle 13, r18d, r27d
	ctestle 5, r30, r29
	ctestle 9, qword [eax+1], rax
	ctestle 0, qword [eax+64], rsi
	ctestle 12, qword [eax+1], -478784017
	ctestle 12, qword [eax+64], -45324884
	ctestle 8, [0xe3f], rax
	ctestle 7, [0x70b], 238658793
%endif
