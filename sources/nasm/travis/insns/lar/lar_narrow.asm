	lar si, bx
	lar cx, bp
	lar ebp, cx
	lar edx, cx

%ifdef ERROR
	lar rax, dx
	lar rbx, word [0xd68]
	lar r11w, r12w
	lar r11d, r11w
	lar r9, r15w
	lar r28w, r31w
	lar r28d, r23w
	lar r16, r22w
	lar rax, word [eax+1]
	lar rdx, word [eax+64]
	lar rcx, [0xe95]
%endif
