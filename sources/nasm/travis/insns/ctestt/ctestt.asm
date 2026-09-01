default rel
	ctestt 4, dl, bl
	ctestt 0, bl, dl
	ctestt 1, word [0x4f7], ax
	ctestt 9, ax, bp
	ctestt 7, esi, ecx
	ctestt 11, edi, ebx
	ctestt 1, rdx, rdx
	ctestt 4, rdi, rbx
	ctestt 15, r14b, r12b
	ctestt 1, r15w, r13w
	ctestt 2, r9d, r14d
	ctestt 1, r11, r9
	ctestt 9, r16b, r23b
	ctestt 2, r19w, r19w
	ctestt 5, r19d, r20d
	ctestt 0, r27, r20
	ctestt 3, byte [eax+1], al
	ctestt 3, byte [eax+64], al
	ctestt 6, word [eax+1], ax
	ctestt 14, word [eax+64], dx
	ctestt 8, dword [eax+1], ebp
	ctestt 3, dword [eax+64], ebx
	ctestt 14, qword [eax+1], rbp
	ctestt 8, qword [eax+64], rsi
	ctestt 6, byte [eax+1], 0x95
	ctestt 6, byte [eax+64], 0xe2
	ctestt 8, word [eax+1], 0xaf63
	ctestt 0, word [eax+64], 0xaeb3
	ctestt 3, dword [eax+1], 0x18f278c8
	ctestt 10, dword [eax+64], 0x35e16bc1
	ctestt 2, qword [eax+1], -222885416
	ctestt 0, qword [eax+64], 363737796
	ctestt 3, [0xfb4], bl
	ctestt 7, [0x73d], bp
	ctestt 0, [0xa82], ecx
	ctestt 11, [0x8e5], rax
	ctestt 0, [0x8ac], 0xc7
	ctestt 12, [0xb47], 0xfbfd
	ctestt 0, [0x5ad], 0x377ba08a
	ctestt 11, [0xb2c], -265473866
