	not dl
	not byte [0x314]
	not di
	not di
	not ebx
	not ebp
	not qword [0x72b]
	not qword [0xce5]

%ifdef ERROR
	not r13b
	not r8w
	not r12d
	not r9
	not r31b
	not r18w
	not r24d
	not r28
	not rax, qword [eax+1]
	not rdx, qword [eax+64]
	not rdi, [0x4fe]
%endif
