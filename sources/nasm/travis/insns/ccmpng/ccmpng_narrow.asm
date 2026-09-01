	ccmpng 14, cl, bl
	ccmpng 2, byte [0x5e9], dl
	ccmpng 12, bx, dx
	ccmpng 8, di, dx
	ccmpng 2, eax, ebx
	ccmpng 6, ebx, edx

%ifdef ERROR
	ccmpng 13, qword [0x635], rcx
	ccmpng 14, qword [0x725], rdi
	ccmpng 6, r9b, r15b
	ccmpng 1, r9w, r15w
	ccmpng 6, r8d, r9d
	ccmpng 6, r14, r12
	ccmpng 8, r29b, r22b
	ccmpng 14, r27w, r23w
	ccmpng 7, r16d, r23d
	ccmpng 13, r29, r19
	ccmpng 0, qword [eax+1], rdx
	ccmpng 15, qword [eax+64], rbp
	ccmpng 11, rbp, qword [eax+1]
	ccmpng 2, rbx, qword [eax+64]
	ccmpng 9, qword [eax+1], -114
	ccmpng 2, qword [eax+64], -59
	ccmpng 6, qword [eax+1], -178735910
	ccmpng 14, qword [eax+64], 39042579
	ccmpng 7, [0x484], rdx
	ccmpng 5, rax, [0xd61]
	ccmpng 4, [0xa6b], -47
	ccmpng 3, [0x6f3], 440372382
%endif
