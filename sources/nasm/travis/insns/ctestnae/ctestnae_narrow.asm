	ctestnae 0, al, dl
	ctestnae 3, cl, bl
	ctestnae 8, word [0x795], dx
	ctestnae 9, dx, bp
	ctestnae 12, esi, ecx
	ctestnae 15, ebp, edx

%ifdef ERROR
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
	ctestnae 13, qword [eax+1], rdi
	ctestnae 13, qword [eax+64], rdi
	ctestnae 10, qword [eax+1], 185460660
	ctestnae 10, qword [eax+64], 136551516
	ctestnae 1, [0x144], rdi
	ctestnae 0, [0xdf8], 471649402
%endif
