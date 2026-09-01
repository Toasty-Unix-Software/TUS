	ctestne 7, bl, dl
	ctestne 5, byte [0xf01], al
	ctestne 10, word [0xa59], ax
	ctestne 4, ax, dx
	ctestne 9, ebp, eax
	ctestne 15, ebp, ecx

%ifdef ERROR
	ctestne 5, rbx, rax
	ctestne 1, rdi, rdi
	ctestne 8, r15b, r9b
	ctestne 14, r11w, r14w
	ctestne 15, r12d, r14d
	ctestne 13, r12, r12
	ctestne 13, r27b, r31b
	ctestne 7, r31w, r22w
	ctestne 4, r30d, r29d
	ctestne 14, r21, r30
	ctestne 9, qword [eax+1], rbx
	ctestne 2, qword [eax+64], rdi
	ctestne 6, qword [eax+1], 431879569
	ctestne 9, qword [eax+64], -269126422
	ctestne 8, [0x8c3], rdx
	ctestne 7, [0x7a3], -285879460
%endif
