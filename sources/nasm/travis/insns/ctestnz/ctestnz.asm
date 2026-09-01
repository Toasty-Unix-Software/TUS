default rel
	ctestnz 10, cl, al
	ctestnz 11, byte [0x6b1], al
	ctestnz 0, word [0x7e8], dx
	ctestnz 5, bx, dx
	ctestnz 2, esi, esi
	ctestnz 11, ebp, esi
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
	ctestnz 14, byte [eax+1], bl
	ctestnz 5, byte [eax+64], cl
	ctestnz 11, word [eax+1], si
	ctestnz 4, word [eax+64], cx
	ctestnz 1, dword [eax+1], eax
	ctestnz 3, dword [eax+64], ecx
	ctestnz 3, qword [eax+1], rsi
	ctestnz 13, qword [eax+64], rcx
	ctestnz 15, byte [eax+1], 0xc0
	ctestnz 12, byte [eax+64], 0xfa
	ctestnz 7, word [eax+1], 0x31f0
	ctestnz 3, word [eax+64], 0xfafb
	ctestnz 5, dword [eax+1], 0x2540de4b
	ctestnz 11, dword [eax+64], 0x2ccff76f
	ctestnz 5, qword [eax+1], 76994103
	ctestnz 13, qword [eax+64], 508969010
	ctestnz 15, [0xd09], al
	ctestnz 2, [0x30c], ax
	ctestnz 5, [0x9f6], ebp
	ctestnz 3, [0xb48], rsi
	ctestnz 4, [0xc07], 0x5e
	ctestnz 6, [0x8ed], 0x9dd6
	ctestnz 12, [0xd03], 0x1530029e
	ctestnz 14, [0xebe], -262786085
