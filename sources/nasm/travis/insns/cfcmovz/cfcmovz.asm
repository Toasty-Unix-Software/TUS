default rel
	cfcmovz word [0xf44], di
	cfcmovz word [0x5c7], bx
	cfcmovz ebp, eax
	cfcmovz dword [0xe6c], eax
	cfcmovz rbx, rbx
	cfcmovz rsi, rdx
	cfcmovz cx, bx
	cfcmovz dx, bx
	cfcmovz r8w, r14w
	cfcmovz r9d, r10d
	cfcmovz r10, r15
	cfcmovz r14w, r11w
	cfcmovz r21w, r17w
	cfcmovz r26d, r28d
	cfcmovz r29, r24
	cfcmovz r31w, r18w
	cfcmovz word [eax+1], si
	cfcmovz word [eax+64], bp
	cfcmovz dword [eax+1], esi
	cfcmovz dword [eax+64], esi
	cfcmovz qword [eax+1], rcx
	cfcmovz qword [eax+64], rcx
	cfcmovz bx, word [eax+1]
	cfcmovz bx, word [eax+64]
	cfcmovz ebp, dword [eax+1]
	cfcmovz edx, dword [eax+64]
	cfcmovz rdx, qword [eax+1]
	cfcmovz rbp, qword [eax+64]
	cfcmovz ax, cx, word [eax+1]
	cfcmovz si, cx, word [eax+64]
	cfcmovz ebx, eax, dword [eax+1]
	cfcmovz ebp, edi, dword [eax+64]
	cfcmovz rdx, rsi, qword [eax+1]
	cfcmovz rax, rcx, qword [eax+64]
	cfcmovz [0x113], bx
	cfcmovz [0xa46], edi
	cfcmovz [0xa75], rdi
	cfcmovz di, [0x6d5]
	cfcmovz ebp, [0xde6]
	cfcmovz rbp, [0x8dc]
	cfcmovz ax, ax, [0xae2]
	cfcmovz ebx, eax, [0x323]
	cfcmovz rbx, rcx, [0x429]
