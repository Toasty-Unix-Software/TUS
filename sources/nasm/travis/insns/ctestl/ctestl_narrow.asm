	ctestl 12, byte [0x186], cl
	ctestl 3, byte [0x5e7], dl
	ctestl 7, bp, ax
	ctestl 7, word [0xbf1], dx
	ctestl 0, edx, esi
	ctestl 14, edi, ebx

%ifdef ERROR
	ctestl 7, rdx, rbp
	ctestl 1, qword [0xab6], rbp
	ctestl 5, r8b, r13b
	ctestl 11, r15w, r14w
	ctestl 8, r8d, r9d
	ctestl 5, r11, r15
	ctestl 4, r21b, r30b
	ctestl 5, r16w, r23w
	ctestl 3, r23d, r25d
	ctestl 12, r21, r24
	ctestl 15, qword [eax+1], rbp
	ctestl 1, qword [eax+64], rbx
	ctestl 9, qword [eax+1], 294558070
	ctestl 13, qword [eax+64], 361047785
	ctestl 8, [0xe77], rsi
	ctestl 10, [0x4cd], 356764616
%endif
