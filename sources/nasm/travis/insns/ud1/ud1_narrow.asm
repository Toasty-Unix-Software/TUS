	ud1 si, si
	ud1 ax, cx
	ud1 esi, dword [0xc08]
	ud1 ecx, ebp
	ud1
	ud1

%ifdef ERROR
	ud1 rbx, rax
	ud1 rdi, qword [0xc3f]
	ud1 r14w, r12w
	ud1 r10d, r15d
	ud1 r15, r10
	ud1 r18w, r28w
	ud1 r21d, r23d
	ud1 r22, r22
	ud1 rax, qword [eax+1]
	ud1 rbp, qword [eax+64]
	ud1 rbp, [0x995]
%endif
