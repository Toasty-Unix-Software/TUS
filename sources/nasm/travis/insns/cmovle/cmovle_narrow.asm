	cmovle bx, di
	cmovle cx, bx
	cmovle ebp, edi
	cmovle ebp, dword [0xfef]
	cmovle bp, bp, ax
	cmovle cx, ax, ax

%ifdef ERROR
	cmovle rbp, rbx
	cmovle rdi, qword [0x1f0]
	cmovle r13w, r15w
	cmovle r9d, r8d
	cmovle r9, r12
	cmovle r14w, r10w, r8w
	cmovle r21w, r30w
	cmovle r26d, r26d
	cmovle r26, r17
	cmovle r21w, r30w, r28w
	cmovle rdx, qword [eax+1]
	cmovle rbx, qword [eax+64]
	cmovle rbx, rdx, qword [eax+1]
	cmovle rbx, rbp, qword [eax+64]
	cmovle rbp, [0xbe3]
	cmovle rsi, rbp, [0x48e]
%endif
