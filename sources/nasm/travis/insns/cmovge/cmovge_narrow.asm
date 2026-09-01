	cmovge dx, si
	cmovge bp, ax
	cmovge edi, dword [0x3e3]
	cmovge edx, ecx
	cmovge di, di, bp
	cmovge bp, si, word [0x84a]

%ifdef ERROR
	cmovge rcx, rbp
	cmovge rdi, rbp
	cmovge r11w, r12w
	cmovge r10d, r14d
	cmovge r9, r13
	cmovge r8w, r10w, r13w
	cmovge r18w, r21w
	cmovge r21d, r21d
	cmovge r17, r24
	cmovge r28w, r27w, r20w
	cmovge rdx, qword [eax+1]
	cmovge rbx, qword [eax+64]
	cmovge rdx, rax, qword [eax+1]
	cmovge rsi, rdi, qword [eax+64]
	cmovge rcx, [0x8dc]
	cmovge rsi, rdi, [0x5aa]
%endif
