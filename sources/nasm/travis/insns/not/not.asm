default rel
	not dl
	not byte [0x314]
	not di
	not di
	not ebx
	not ebp
	not qword [0x72b]
	not qword [0xce5]
	not r13b
	not r8w
	not r12d
	not r9
	not r31b
	not r18w
	not r24d
	not r28
	not byte [eax+1]
	not byte [eax+64]
	not word [eax+1]
	not word [eax+64]
	not dword [eax+1]
	not dword [eax+64]
	not qword [eax+1]
	not qword [eax+64]
	not dl, byte [eax+1]
	not bl, byte [eax+64]
	not cx, word [eax+1]
	not si, word [eax+64]
	not ebx, dword [eax+1]
	not eax, dword [eax+64]
	not rax, qword [eax+1]
	not rdx, qword [eax+64]
	not [0xc05]
	not [0x149]
	not [0xf40]
	not [0x1eb]
	not bl, [0xdc9]
	not ax, [0xf61]
	not edx, [0x6f2]
	not rdi, [0x4fe]
