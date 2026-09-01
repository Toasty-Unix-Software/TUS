default rel
	ctestg 13, byte [0xa82], bl
	ctestg 14, dl, cl
	ctestg 13, si, di
	ctestg 1, di, dx
	ctestg 12, ebp, ebp
	ctestg 2, dword [0x4c2], ecx
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
	ctestg 12, byte [eax+1], cl
	ctestg 4, byte [eax+64], cl
	ctestg 1, word [eax+1], si
	ctestg 15, word [eax+64], bp
	ctestg 8, dword [eax+1], ecx
	ctestg 13, dword [eax+64], ecx
	ctestg 11, qword [eax+1], rax
	ctestg 7, qword [eax+64], rsi
	ctestg 11, byte [eax+1], 0xb3
	ctestg 4, byte [eax+64], 0x55
	ctestg 4, word [eax+1], 0xf021
	ctestg 12, word [eax+64], 0xd334
	ctestg 13, dword [eax+1], 0xf02a2b9
	ctestg 12, dword [eax+64], 0x336807a7
	ctestg 0, qword [eax+1], 201876078
	ctestg 10, qword [eax+64], 178561052
	ctestg 10, [0xfd7], bl
	ctestg 15, [0xfee], si
	ctestg 6, [0xb2e], edi
	ctestg 10, [0xa76], rcx
	ctestg 14, [0x4ed], 0x3a
	ctestg 10, [0x84e], 0x812
	ctestg 6, [0xbf6], 0x2d5eac68
	ctestg 8, [0x174], -36403440
