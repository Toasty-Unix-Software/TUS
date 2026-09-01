	cmovne bx, dx
	cmovne bp, word [0x37f]
	cmovne esi, esi
	cmovne edx, ecx
	cmovne dx, bp, bp
	cmovne ax, di, cx

%ifdef ERROR
	cmovne rbx, qword [0x668]
	cmovne rax, qword [0xbd5]
	cmovne r11w, r13w
	cmovne r13d, r11d
	cmovne r8, r12
	cmovne r13w, r11w, r11w
	cmovne r24w, r28w
	cmovne r25d, r27d
	cmovne r16, r27
	cmovne r16w, r21w, r30w
	cmovne rdx, qword [eax+1]
	cmovne rbx, qword [eax+64]
	cmovne rbp, rbp, qword [eax+1]
	cmovne rax, rax, qword [eax+64]
	cmovne rbp, [0x8a2]
	cmovne rsi, rdx, [0x7f1]
%endif
