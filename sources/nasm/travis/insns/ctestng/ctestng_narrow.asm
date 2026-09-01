	ctestng 2, cl, al
	ctestng 7, dl, bl
	ctestng 8, dx, cx
	ctestng 5, di, ax
	ctestng 13, esi, edi
	ctestng 2, dword [0x4a9], ecx

%ifdef ERROR
	ctestng 1, rbp, rcx
	ctestng 0, qword [0xeac], rdx
	ctestng 15, r15b, r13b
	ctestng 13, r8w, r8w
	ctestng 7, r13d, r10d
	ctestng 11, r15, r14
	ctestng 14, r30b, r16b
	ctestng 8, r19w, r17w
	ctestng 9, r31d, r26d
	ctestng 10, r22, r24
	ctestng 12, qword [eax+1], rbp
	ctestng 0, qword [eax+64], rbp
	ctestng 7, qword [eax+1], -461464867
	ctestng 12, qword [eax+64], -114605603
	ctestng 10, [0xec9], rax
	ctestng 5, [0xe83], -330602137
%endif
