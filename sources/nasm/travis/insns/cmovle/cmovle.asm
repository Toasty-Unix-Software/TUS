default rel
	cmovle bx, di
	cmovle cx, bx
	cmovle ebp, edi
	cmovle ebp, dword [0xfef]
	cmovle rbp, rbx
	cmovle rdi, qword [0x1f0]
	cmovle bp, bp, ax
	cmovle cx, ax, ax
	cmovle r13w, r15w
	cmovle r9d, r8d
	cmovle r9, r12
	cmovle r14w, r10w, r8w
	cmovle r21w, r30w
	cmovle r26d, r26d
	cmovle r26, r17
	cmovle r21w, r30w, r28w
	cmovle si, word [eax+1]
	cmovle di, word [eax+64]
	cmovle ebx, dword [eax+1]
	cmovle esi, dword [eax+64]
	cmovle rdx, qword [eax+1]
	cmovle rbx, qword [eax+64]
	cmovle dx, ax, word [eax+1]
	cmovle cx, cx, word [eax+64]
	cmovle ebx, edx, dword [eax+1]
	cmovle edx, eax, dword [eax+64]
	cmovle rbx, rdx, qword [eax+1]
	cmovle rbx, rbp, qword [eax+64]
	cmovle di, [0xc80]
	cmovle edi, [0xe13]
	cmovle rbp, [0xbe3]
	cmovle di, bx, [0xd6c]
	cmovle eax, ecx, [0xf57]
	cmovle rsi, rbp, [0x48e]
