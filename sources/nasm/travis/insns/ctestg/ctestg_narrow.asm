	ctestg 13, byte [0xa82], bl
	ctestg 14, dl, cl
	ctestg 13, si, di
	ctestg 1, di, dx
	ctestg 12, ebp, ebp
	ctestg 2, dword [0x4c2], ecx

%ifdef ERROR
	ctestg 8, rax, rbx
	ctestg 9, rsi, rdx
	ctestg 9, r8b, r8b
	ctestg 14, r12w, r15w
	ctestg 2, r9d, r15d
	ctestg 15, r10, r11
	ctestg 12, r21b, r30b
	ctestg 11, r16w, r24w
	ctestg 15, r19d, r20d
	ctestg 1, r30, r28
	ctestg 11, qword [eax+1], rax
	ctestg 7, qword [eax+64], rsi
	ctestg 0, qword [eax+1], 201876078
	ctestg 10, qword [eax+64], 178561052
	ctestg 10, [0xa76], rcx
	ctestg 8, [0x174], -36403440
%endif
