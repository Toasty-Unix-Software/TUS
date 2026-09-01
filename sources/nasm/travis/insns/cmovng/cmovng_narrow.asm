	cmovng cx, cx
	cmovng si, word [0xf0f]
	cmovng ebp, dword [0x812]
	cmovng eax, ecx
	cmovng di, dx, dx
	cmovng cx, bp, word [0xd88]

%ifdef ERROR
	cmovng rbp, qword [0x636]
	cmovng rbp, rdi
	cmovng r8w, r8w
	cmovng r8d, r10d
	cmovng r15, r15
	cmovng r12w, r10w, r9w
	cmovng r20w, r25w
	cmovng r26d, r19d
	cmovng r27, r26
	cmovng r17w, r26w, r23w
	cmovng rsi, qword [eax+1]
	cmovng rdi, qword [eax+64]
	cmovng rax, rbx, qword [eax+1]
	cmovng rbp, rcx, qword [eax+64]
	cmovng rbp, [0xe67]
	cmovng rcx, rbx, [0xbb5]
%endif
