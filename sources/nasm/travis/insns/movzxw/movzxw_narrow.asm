	movzxw si, dx
	movzxw di, word [0x550]
	movzxw ebx, bx
	movzxw ebp, si

%ifdef ERROR
	movzxw rsi, ax
	movzxw rax, si
	movzxw r15w, r10w
	movzxw r15d, r13w
	movzxw r13, r12w
	movzxw r21w, r31w
	movzxw r20d, r28w
	movzxw r30, r24w
	movzxw rsi, word [eax+1]
	movzxw rsi, word [eax+64]
	movzxw rbx, [0xb07]
%endif
