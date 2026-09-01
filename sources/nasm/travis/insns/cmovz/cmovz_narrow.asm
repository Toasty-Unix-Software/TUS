	cmovz di, di
	cmovz di, bx
	cmovz eax, dword [0xf2d]
	cmovz edx, dword [0x409]
	cmovz ax, dx, word [0x3d7]
	cmovz bp, bp, word [0x8f0]

%ifdef ERROR
	cmovz rdx, rbp
	cmovz rbx, qword [0x756]
	cmovz r8w, r9w
	cmovz r9d, r9d
	cmovz r12, r14
	cmovz r11w, r12w, r13w
	cmovz r24w, r22w
	cmovz r27d, r22d
	cmovz r20, r17
	cmovz r18w, r18w, r27w
	cmovz rsi, qword [eax+1]
	cmovz rax, qword [eax+64]
	cmovz rbx, rbx, qword [eax+1]
	cmovz rax, rsi, qword [eax+64]
	cmovz rsi, [0x471]
	cmovz rdx, rbx, [0xb45]
%endif
