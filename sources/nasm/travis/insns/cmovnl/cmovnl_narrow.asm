	cmovnl di, si
	cmovnl di, dx
	cmovnl edi, eax
	cmovnl esi, edx
	cmovnl di, di, word [0x436]
	cmovnl si, bx, bp

%ifdef ERROR
	cmovnl rdx, qword [0x9d3]
	cmovnl rdi, rcx
	cmovnl r8w, r11w
	cmovnl r14d, r13d
	cmovnl r12, r9
	cmovnl r12w, r13w, r8w
	cmovnl r31w, r24w
	cmovnl r21d, r18d
	cmovnl r31, r27
	cmovnl r22w, r29w, r29w
	cmovnl rcx, qword [eax+1]
	cmovnl rcx, qword [eax+64]
	cmovnl rcx, rbx, qword [eax+1]
	cmovnl rsi, rbx, qword [eax+64]
	cmovnl rbp, [0xe1f]
	cmovnl rdx, rax, [0x2cf]
%endif
