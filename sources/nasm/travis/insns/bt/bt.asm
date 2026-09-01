default rel
	bt ax, di
	bt ax, dx
	bt dword [0xf36], ebx
	bt ebp, ebp
	bt rbx, rsi
	bt rdi, rbx
	bt cx, 0x6f
	bt word [0x900], 0x2e
	bt r15w, r9w
	bt r10d, r14d
	bt r8, r13
	bt r8w, 0x13
	bt r28w, r27w
	bt r22d, r16d
	bt r21, r25
	bt r17w, 0xd9
	bt word [eax+1], di
	bt word [eax+64], ax
	bt dword [eax+1], ecx
	bt dword [eax+64], ebx
	bt qword [eax+1], rsi
	bt qword [eax+64], rbx
	bt word [eax+1], 0xe8
	bt word [eax+64], 0x1e
	bt dword [eax+1], 0x1d
	bt dword [eax+64], 0x9b
	bt qword [eax+1], 0x4b
	bt qword [eax+64], 0x69
	bt [0x48b], ax
	bt [0x5ca], esi
	bt [0xef2], rax
	bt [0x515], 0x4
	bt [0x89b], 0xeb
	bt [0x95c], 0x96
