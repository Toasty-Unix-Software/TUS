default rel
	cmpbxadd dword [0xf17], ebp, ebx
	cmpbxadd dword [0xe38], edi, eax
	cmpbxadd qword [0xb3b], rax, rcx
	cmpbxadd qword [0x652], rcx, rdx
	cmpbxadd dword [0xb54], r12d, r14d
	cmpbxadd qword [0xf8e], r10, r15
	cmpbxadd dword [0xade], r25d, r18d
	cmpbxadd qword [0xa37], r31, r28
	cmpbxadd dword [eax+1], ebp, edx
	cmpbxadd dword [eax+64], edx, ebp
	cmpbxadd qword [eax+1], rdx, rbx
	cmpbxadd qword [eax+64], rcx, rcx
	cmpbxadd [0x5ad], ebx, ebx
	cmpbxadd [0x676], rcx, rcx
