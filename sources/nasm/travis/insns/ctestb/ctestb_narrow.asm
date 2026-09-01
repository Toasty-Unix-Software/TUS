	ctestb 5, dl, dl
	ctestb 15, al, bl
	ctestb 3, di, ax
	ctestb 13, word [0x733], bx
	ctestb 11, ebp, ebx
	ctestb 15, ecx, eax

%ifdef ERROR
	ctestb 14, qword [0xc65], rax
	ctestb 3, rax, rsi
	ctestb 10, r8b, r10b
	ctestb 15, r9w, r14w
	ctestb 15, r8d, r10d
	ctestb 5, r8, r12
	ctestb 7, r30b, r21b
	ctestb 12, r24w, r25w
	ctestb 10, r16d, r19d
	ctestb 0, r29, r28
	ctestb 13, qword [eax+1], rdx
	ctestb 11, qword [eax+64], rdx
	ctestb 4, qword [eax+1], 138111268
	ctestb 9, qword [eax+64], -385562069
	ctestb 11, [0xcaf], rsi
	ctestb 15, [0x68c], 453872912
%endif
