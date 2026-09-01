	cmovna bx, ax
	cmovna cx, dx
	cmovna esi, ebx
	cmovna eax, edi
	cmovna si, dx, bp
	cmovna cx, di, bx

%ifdef ERROR
	cmovna rdi, qword [0xa48]
	cmovna rbp, rax
	cmovna r12w, r10w
	cmovna r10d, r11d
	cmovna r13, r15
	cmovna r14w, r15w, r13w
	cmovna r24w, r17w
	cmovna r19d, r21d
	cmovna r28, r23
	cmovna r31w, r19w, r31w
	cmovna rax, qword [eax+1]
	cmovna rsi, qword [eax+64]
	cmovna rbx, rdx, qword [eax+1]
	cmovna rbp, rbp, qword [eax+64]
	cmovna rcx, [0x2a2]
	cmovna rcx, rdx, [0x412]
%endif
