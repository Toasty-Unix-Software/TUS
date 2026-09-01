	cmova bx, word [0x36a]
	cmova bx, bp
	cmova ecx, eax
	cmova ecx, dword [0x19e]
	cmova di, bx, word [0x39c]
	cmova di, dx, word [0xc2c]

%ifdef ERROR
	cmova rdi, rax
	cmova rsi, qword [0x547]
	cmova r11w, r11w
	cmova r10d, r12d
	cmova r13, r14
	cmova r11w, r15w, r11w
	cmova r27w, r22w
	cmova r28d, r17d
	cmova r28, r22
	cmova r20w, r21w, r19w
	cmova rbx, qword [eax+1]
	cmova rax, qword [eax+64]
	cmova rsi, rax, qword [eax+1]
	cmova rsi, rbp, qword [eax+64]
	cmova rax, [0xb68]
	cmova rax, rdx, [0xac3]
%endif
