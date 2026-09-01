	cfcmovz word [0xf44], di
	cfcmovz word [0x5c7], bx
	cfcmovz ebp, eax
	cfcmovz dword [0xe6c], eax
	cfcmovz cx, bx
	cfcmovz dx, bx

%ifdef ERROR
	cfcmovz rbx, rbx
	cfcmovz rsi, rdx
	cfcmovz r8w, r14w
	cfcmovz r9d, r10d
	cfcmovz r10, r15
	cfcmovz r14w, r11w
	cfcmovz r21w, r17w
	cfcmovz r26d, r28d
	cfcmovz r29, r24
	cfcmovz r31w, r18w
	cfcmovz qword [eax+1], rcx
	cfcmovz qword [eax+64], rcx
	cfcmovz rdx, qword [eax+1]
	cfcmovz rbp, qword [eax+64]
	cfcmovz rdx, rsi, qword [eax+1]
	cfcmovz rax, rcx, qword [eax+64]
	cfcmovz [0xa75], rdi
	cfcmovz rbp, [0x8dc]
	cfcmovz rbx, rcx, [0x429]
%endif
