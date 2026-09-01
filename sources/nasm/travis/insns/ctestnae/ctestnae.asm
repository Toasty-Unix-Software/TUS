default rel
	ctestnae 0, al, dl
	ctestnae 3, cl, bl
	ctestnae 8, word [0x795], dx
	ctestnae 9, dx, bp
	ctestnae 12, esi, ecx
	ctestnae 15, ebp, edx
	ctestnae 4, rdx, rsi
	ctestnae 11, rsi, rbx
	ctestnae 7, r13b, r11b
	ctestnae 9, r9w, r12w
	ctestnae 14, r10d, r12d
	ctestnae 8, r13, r15
	ctestnae 1, r24b, r18b
	ctestnae 4, r19w, r25w
	ctestnae 11, r24d, r29d
	ctestnae 10, r23, r19
	ctestnae 14, byte [eax+1], dl
	ctestnae 10, byte [eax+64], dl
	ctestnae 3, word [eax+1], cx
	ctestnae 1, word [eax+64], dx
	ctestnae 14, dword [eax+1], ecx
	ctestnae 6, dword [eax+64], edi
	ctestnae 13, qword [eax+1], rdi
	ctestnae 13, qword [eax+64], rdi
	ctestnae 3, byte [eax+1], 0xee
	ctestnae 3, byte [eax+64], 0xe5
	ctestnae 8, word [eax+1], 0xb127
	ctestnae 7, word [eax+64], 0x6f17
	ctestnae 9, dword [eax+1], 0x31c433cd
	ctestnae 0, dword [eax+64], 0x387ca22b
	ctestnae 10, qword [eax+1], 185460660
	ctestnae 10, qword [eax+64], 136551516
	ctestnae 11, [0x8bb], cl
	ctestnae 0, [0xb8a], dx
	ctestnae 14, [0x60e], ebp
	ctestnae 1, [0x144], rdi
	ctestnae 2, [0xa9f], 0x27
	ctestnae 14, [0x52a], 0x3fef
	ctestnae 0, [0x242], 0x27afeb86
	ctestnae 0, [0xdf8], 471649402
