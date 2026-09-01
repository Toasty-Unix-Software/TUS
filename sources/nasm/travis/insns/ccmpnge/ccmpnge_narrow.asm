	ccmpnge 6, cl, bl
	ccmpnge 11, cl, bl
	ccmpnge 1, dx, bp
	ccmpnge 2, bx, di
	ccmpnge 11, edx, ebp
	ccmpnge 9, ebx, ecx

%ifdef ERROR
	ccmpnge 9, rbx, rsi
	ccmpnge 15, qword [0x1c4], rdi
	ccmpnge 15, r14b, r12b
	ccmpnge 14, r15w, r15w
	ccmpnge 4, r14d, r8d
	ccmpnge 8, r12, r10
	ccmpnge 10, r31b, r18b
	ccmpnge 13, r19w, r29w
	ccmpnge 0, r26d, r24d
	ccmpnge 1, r23, r27
	ccmpnge 3, qword [eax+1], rcx
	ccmpnge 5, qword [eax+64], rdi
	ccmpnge 1, rbp, qword [eax+1]
	ccmpnge 10, rbx, qword [eax+64]
	ccmpnge 8, qword [eax+1], -10
	ccmpnge 10, qword [eax+64], 126
	ccmpnge 10, qword [eax+1], 424090048
	ccmpnge 5, qword [eax+64], -460094512
	ccmpnge 5, [0x5fb], rax
	ccmpnge 13, rbp, [0x2e0]
	ccmpnge 7, [0x3e7], 116
	ccmpnge 13, [0x562], 441775119
%endif
