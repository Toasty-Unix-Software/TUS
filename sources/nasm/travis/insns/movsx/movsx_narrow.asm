	movsx ax, al
	movsx ax, al
	movsx eax, ax
	movsx eax, ax
	movsx dx, dl
	movsx cx, cl

%ifdef ERROR
	movsx rax, eax
	movsx rax, eax
	movsx r8w, r14b
	movsx r21w, r31b
	movsx rbp, byte [eax+1]
	movsx rbx, byte [eax+64]
	movsx rcx, word [eax+1]
	movsx rbp, word [eax+64]
	movsx rsi, dword [eax+1]
	movsx rsi, dword [eax+64]
%endif
