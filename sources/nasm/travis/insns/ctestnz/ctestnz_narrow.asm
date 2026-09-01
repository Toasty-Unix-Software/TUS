	ctestnz 10, cl, al
	ctestnz 11, byte [0x6b1], al
	ctestnz 0, word [0x7e8], dx
	ctestnz 5, bx, dx
	ctestnz 2, esi, esi
	ctestnz 11, ebp, esi

%ifdef ERROR
	ctestnz 14, rbx, rdi
	ctestnz 5, rdi, rax
	ctestnz 11, r9b, r9b
	ctestnz 0, r15w, r12w
	ctestnz 0, r13d, r13d
	ctestnz 8, r14, r8
	ctestnz 0, r18b, r16b
	ctestnz 11, r23w, r26w
	ctestnz 7, r20d, r26d
	ctestnz 2, r26, r28
	ctestnz 3, qword [eax+1], rsi
	ctestnz 13, qword [eax+64], rcx
	ctestnz 5, qword [eax+1], 76994103
	ctestnz 13, qword [eax+64], 508969010
	ctestnz 3, [0xb48], rsi
	ctestnz 14, [0xebe], -262786085
%endif
