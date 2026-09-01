	movzxb ax, al
	movzxb cx, dl
	movzxb ebp, al
	movzxb edx, bl

%ifdef ERROR
	movzxb rcx, cl
	movzxb rax, al
	movzxb r9w, r8b
	movzxb r14d, r8b
	movzxb r8, r15b
	movzxb r22w, r23b
	movzxb r28d, r20b
	movzxb r21, r21b
	movzxb rdi, byte [eax+1]
	movzxb rsi, byte [eax+64]
	movzxb rsi, [0x700]
%endif
