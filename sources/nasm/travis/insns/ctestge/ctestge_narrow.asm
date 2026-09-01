	ctestge 5, dl, al
	ctestge 8, dl, cl
	ctestge 3, di, ax
	ctestge 10, word [0xc28], dx
	ctestge 3, edi, ecx
	ctestge 5, ebx, ecx

%ifdef ERROR
	ctestge 9, qword [0x6ce], rcx
	ctestge 2, qword [0x20c], rbx
	ctestge 11, r9b, r9b
	ctestge 4, r9w, r8w
	ctestge 4, r12d, r10d
	ctestge 13, r11, r11
	ctestge 15, r20b, r30b
	ctestge 1, r25w, r24w
	ctestge 12, r26d, r28d
	ctestge 10, r17, r27
	ctestge 2, qword [eax+1], rcx
	ctestge 7, qword [eax+64], rbx
	ctestge 2, qword [eax+1], 376208280
	ctestge 6, qword [eax+64], -4161025
	ctestge 6, [0xa4e], rbp
	ctestge 1, [0xd63], -304702834
%endif
