	cteste 8, byte [0xa31], bl
	cteste 9, dl, cl
	cteste 13, word [0xce6], bp
	cteste 8, word [0xec5], di
	cteste 4, ebp, ebp
	cteste 4, edi, ebx

%ifdef ERROR
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
	cteste 3, qword [eax+1], rdi
	cteste 9, qword [eax+64], rbx
	cteste 5, qword [eax+1], -62967126
	cteste 13, qword [eax+64], -89844277
	cteste 0, [0x65f], rdi
	cteste 7, [0x6df], 494121071
%endif
