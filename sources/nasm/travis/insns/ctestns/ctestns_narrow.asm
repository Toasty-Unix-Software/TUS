	ctestns 15, al, cl
	ctestns 0, cl, dl
	ctestns 11, cx, cx
	ctestns 15, cx, cx
	ctestns 15, esi, ebx
	ctestns 12, edi, esi

%ifdef ERROR
	ctestns 6, qword [0xd8d], rdx
	ctestns 8, qword [0x919], rbx
	ctestns 15, r11b, r9b
	ctestns 5, r8w, r10w
	ctestns 6, r12d, r10d
	ctestns 2, r9, r14
	ctestns 6, r22b, r17b
	ctestns 1, r21w, r19w
	ctestns 12, r20d, r28d
	ctestns 14, r25, r22
	ctestns 9, qword [eax+1], rax
	ctestns 4, qword [eax+64], rsi
	ctestns 7, qword [eax+1], -63057385
	ctestns 10, qword [eax+64], 27044069
	ctestns 0, [0xe85], rcx
	ctestns 11, [0x98a], 319382974
%endif
