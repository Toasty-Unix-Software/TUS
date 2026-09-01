	lsl ax, word [0x289]
	lsl ax, word [0x884]
	lsl ebx, word [0x857]
	lsl eax, word [0x7f1]

%ifdef ERROR
	lsl rdx, word [0xf2e]
	lsl rdx, word [0xb78]
	lsl r10w, r14w
	lsl r14d, r8w
	lsl r8, r14w
	lsl r28w, r28w
	lsl r31d, r31w
	lsl r29, r29w
	lsl rax, word [eax+1]
	lsl rbx, word [eax+64]
	lsl rcx, [0xd84]
%endif
