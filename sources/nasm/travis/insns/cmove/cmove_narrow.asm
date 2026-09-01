	cmove si, dx
	cmove ax, ax
	cmove ebx, ecx
	cmove ebx, esi
	cmove di, si, cx
	cmove di, ax, ax

%ifdef ERROR
	cmove rbp, rdx
	cmove rdi, rbx
	cmove r11w, r9w
	cmove r9d, r8d
	cmove r9, r9
	cmove r12w, r13w, r13w
	cmove r24w, r26w
	cmove r30d, r16d
	cmove r20, r31
	cmove r29w, r16w, r17w
	cmove rbp, qword [eax+1]
	cmove rdx, qword [eax+64]
	cmove rbx, rdi, qword [eax+1]
	cmove rdx, rsi, qword [eax+64]
	cmove rsi, [0xecc]
	cmove rcx, rsi, [0xf31]
%endif
