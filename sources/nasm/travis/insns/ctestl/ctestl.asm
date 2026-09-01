default rel
	ctestl 12, byte [0x186], cl
	ctestl 3, byte [0x5e7], dl
	ctestl 7, bp, ax
	ctestl 7, word [0xbf1], dx
	ctestl 0, edx, esi
	ctestl 14, edi, ebx
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
	ctestl 5, byte [eax+1], al
	ctestl 11, byte [eax+64], dl
	ctestl 11, word [eax+1], dx
	ctestl 11, word [eax+64], di
	ctestl 7, dword [eax+1], edi
	ctestl 14, dword [eax+64], ebx
	ctestl 15, qword [eax+1], rbp
	ctestl 1, qword [eax+64], rbx
	ctestl 7, byte [eax+1], 0x37
	ctestl 5, byte [eax+64], 0x5d
	ctestl 2, word [eax+1], 0x933e
	ctestl 2, word [eax+64], 0x9d3e
	ctestl 8, dword [eax+1], 0x182613fc
	ctestl 4, dword [eax+64], 0x27995619
	ctestl 9, qword [eax+1], 294558070
	ctestl 13, qword [eax+64], 361047785
	ctestl 14, [0xaac], dl
	ctestl 3, [0x4d4], di
	ctestl 1, [0x87d], eax
	ctestl 8, [0xe77], rsi
	ctestl 6, [0x160], 0xbf
	ctestl 9, [0xcc1], 0x9553
	ctestl 13, [0x1bd], 0x14c8df6c
	ctestl 10, [0x4cd], 356764616
