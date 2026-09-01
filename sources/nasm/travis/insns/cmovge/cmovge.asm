default rel
	cmovge dx, si
	cmovge bp, ax
	cmovge edi, dword [0x3e3]
	cmovge edx, ecx
	cmovge rcx, rbp
	cmovge rdi, rbp
	cmovge di, di, bp
	cmovge bp, si, word [0x84a]
	cmovge r11w, r12w
	cmovge r10d, r14d
	cmovge r9, r13
	cmovge r8w, r10w, r13w
	cmovge r18w, r21w
	cmovge r21d, r21d
	cmovge r17, r24
	cmovge r28w, r27w, r20w
	cmovge bx, word [eax+1]
	cmovge bx, word [eax+64]
	cmovge esi, dword [eax+1]
	cmovge eax, dword [eax+64]
	cmovge rdx, qword [eax+1]
	cmovge rbx, qword [eax+64]
	cmovge di, bx, word [eax+1]
	cmovge ax, si, word [eax+64]
	cmovge eax, esi, dword [eax+1]
	cmovge ebp, ebp, dword [eax+64]
	cmovge rdx, rax, qword [eax+1]
	cmovge rsi, rdi, qword [eax+64]
	cmovge bp, [0xf71]
	cmovge ebp, [0x689]
	cmovge rcx, [0x8dc]
	cmovge ax, si, [0x145]
	cmovge edx, ebp, [0x67e]
	cmovge rsi, rdi, [0x5aa]
