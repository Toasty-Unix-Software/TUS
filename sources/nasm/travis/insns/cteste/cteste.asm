default rel
	cteste 8, byte [0xa31], bl
	cteste 9, dl, cl
	cteste 13, word [0xce6], bp
	cteste 8, word [0xec5], di
	cteste 4, ebp, ebp
	cteste 4, edi, ebx
	cteste 0, rax, rbp
	cteste 6, qword [0xb9f], rax
	cteste 11, r13b, r9b
	cteste 9, r14w, r15w
	cteste 0, r9d, r8d
	cteste 10, r9, r10
	cteste 15, r28b, r28b
	cteste 3, r19w, r30w
	cteste 11, r22d, r25d
	cteste 1, r20, r26
	cteste 6, byte [eax+1], bl
	cteste 7, byte [eax+64], cl
	cteste 7, word [eax+1], di
	cteste 2, word [eax+64], bx
	cteste 7, dword [eax+1], ecx
	cteste 7, dword [eax+64], ebp
	cteste 3, qword [eax+1], rdi
	cteste 9, qword [eax+64], rbx
	cteste 1, byte [eax+1], 0x21
	cteste 5, byte [eax+64], 0x20
	cteste 1, word [eax+1], 0x21b1
	cteste 15, word [eax+64], 0xa6f
	cteste 12, dword [eax+1], 0x3f4166cf
	cteste 8, dword [eax+64], 0x20e63b71
	cteste 5, qword [eax+1], -62967126
	cteste 13, qword [eax+64], -89844277
	cteste 14, [0xb8c], al
	cteste 10, [0x808], bp
	cteste 2, [0x5a5], ebp
	cteste 0, [0x65f], rdi
	cteste 13, [0x1c6], 0x37
	cteste 15, [0x47b], 0x2953
	cteste 0, [0x18c], 0xf6c3e64
	cteste 7, [0x6df], 494121071
