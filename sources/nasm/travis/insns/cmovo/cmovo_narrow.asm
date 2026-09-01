	cmovo di, cx
	cmovo bp, si
	cmovo esi, edx
	cmovo esi, eax
	cmovo si, si, word [0x925]
	cmovo ax, bx, di

%ifdef ERROR
	cmovo rcx, qword [0xaf8]
	cmovo rcx, qword [0x64c]
	cmovo r10w, r10w
	cmovo r14d, r12d
	cmovo r14, r13
	cmovo r9w, r8w, r10w
	cmovo r16w, r24w
	cmovo r31d, r30d
	cmovo r19, r19
	cmovo r22w, r25w, r26w
	cmovo rdi, qword [eax+1]
	cmovo rcx, qword [eax+64]
	cmovo rbx, rdx, qword [eax+1]
	cmovo rdx, rbx, qword [eax+64]
	cmovo rsi, [0xc69]
	cmovo rbp, rdx, [0xf46]
%endif
