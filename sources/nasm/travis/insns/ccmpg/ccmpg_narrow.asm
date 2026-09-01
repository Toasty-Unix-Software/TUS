	ccmpg 3, byte [0x4d4], bl
	ccmpg 3, dl, dl
	ccmpg 11, di, di
	ccmpg 1, cx, di
	ccmpg 4, edx, ebx
	ccmpg 4, edi, ebp

%ifdef ERROR
	ccmpg 0, rbx, rsi
	ccmpg 15, rdi, rsi
	ccmpg 6, r12b, r9b
	ccmpg 6, r11w, r14w
	ccmpg 0, r14d, r9d
	ccmpg 0, r9, r8
	ccmpg 15, r16b, r28b
	ccmpg 6, r30w, r21w
	ccmpg 9, r31d, r24d
	ccmpg 1, r28, r27
	ccmpg 13, qword [eax+1], rdx
	ccmpg 3, qword [eax+64], rdi
	ccmpg 1, rax, qword [eax+1]
	ccmpg 2, rsi, qword [eax+64]
	ccmpg 5, qword [eax+1], 87
	ccmpg 8, qword [eax+64], 0
	ccmpg 7, qword [eax+1], 429263645
	ccmpg 9, qword [eax+64], 220487927
	ccmpg 1, [0x68a], rdx
	ccmpg 3, rbp, [0x49e]
	ccmpg 15, [0xd83], 92
	ccmpg 15, [0x696], 6647136
%endif
