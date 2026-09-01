default rel
	rol byte [0x39d], 1
	rol byte [0x91c], 1
	rol word [0xf53], 1
	rol ax, 1
	rol dword [0x537], 1
	rol edi, 1
	rol rsi, 1
	rol rdi, 1
	rol r8b, 1
	rol r13w, 1
	rol r12d, 1
	rol r12, 1
	rol r26b, 1
	rol r23w, 1
	rol r30d, 1
	rol r20, 1
	rol byte [eax+1], 1
	rol byte [eax+64], 1
	rol word [eax+1], 1
	rol word [eax+64], 1
	rol dword [eax+1], 1
	rol dword [eax+64], 1
	rol qword [eax+1], 1
	rol qword [eax+64], 1
	rol byte [eax+1], cl
	rol byte [eax+64], cl
	rol word [eax+1], cl
	rol word [eax+64], cl
	rol dword [eax+1], cl
	rol dword [eax+64], cl
	rol qword [eax+1], cl
	rol qword [eax+64], cl
	rol byte [eax+1], cx
	rol byte [eax+64], cx
	rol word [eax+1], cx
	rol word [eax+64], cx
	rol dword [eax+1], cx
	rol dword [eax+64], cx
	rol qword [eax+1], cx
	rol qword [eax+64], cx
	rol byte [eax+1], ecx
	rol byte [eax+64], ecx
	rol word [eax+1], ecx
	rol word [eax+64], ecx
	rol dword [eax+1], ecx
	rol dword [eax+64], ecx
	rol qword [eax+1], ecx
	rol qword [eax+64], ecx
	rol byte [eax+1], 0x94
	rol byte [eax+64], 0xbf
	rol word [eax+1], 0x3a
	rol word [eax+64], 0x1f
	rol dword [eax+1], 0xc
	rol dword [eax+64], 0x8e
	rol qword [eax+1], 0xc
	rol qword [eax+64], 0x7c
	rol eax, dword [eax+1], 1
	rol edi, dword [eax+64], 1
	rol rdi, qword [eax+1], 1
	rol rbp, qword [eax+64], 1
	rol al, byte [eax+1], 1
	rol al, byte [eax+64], 1
	rol ax, word [eax+1], 1
	rol dx, word [eax+64], 1
	rol ebp, dword [eax+1], 1
	rol ecx, dword [eax+64], 1
	rol rdx, qword [eax+1], 1
	rol rbx, qword [eax+64], 1
	rol cl, byte [eax+1], cl
	rol cl, byte [eax+64], cl
	rol si, word [eax+1], cl
	rol si, word [eax+64], cl
	rol ecx, dword [eax+1], cl
	rol edx, dword [eax+64], cl
	rol rdx, qword [eax+1], cl
	rol rbx, qword [eax+64], cl
	rol cl, byte [eax+1], cx
	rol al, byte [eax+64], cx
	rol bx, word [eax+1], cx
	rol ax, word [eax+64], cx
	rol ecx, dword [eax+1], cx
	rol ebx, dword [eax+64], cx
	rol rbx, qword [eax+1], cx
	rol rdx, qword [eax+64], cx
	rol cl, byte [eax+1], ecx
	rol al, byte [eax+64], ecx
	rol dx, word [eax+1], ecx
	rol dx, word [eax+64], ecx
	rol eax, dword [eax+1], ecx
	rol edi, dword [eax+64], ecx
	rol rax, qword [eax+1], ecx
	rol rax, qword [eax+64], ecx
	rol bl, byte [eax+1], 0xba
	rol dl, byte [eax+64], 0x9e
	rol di, word [eax+1], 0x82
	rol ax, word [eax+64], 0xf6
	rol ecx, dword [eax+1], 0x51
	rol ebp, dword [eax+64], 0xf2
	rol rdx, qword [eax+1], 0xc
	rol rbp, qword [eax+64], 0x87
	rol [0xfc5], 1
	rol [0x648], 1
	rol [0xdf9], 1
	rol [0xa98], 1
	rol [0xcf7], cl
	rol [0xb84], cl
	rol [0x235], cl
	rol [0xeda], cl
	rol [0xec0], cx
	rol [0x2f8], cx
	rol [0x8a0], cx
	rol [0x545], cx
	rol [0x68a], ecx
	rol [0x5dc], ecx
	rol [0x2a5], ecx
	rol [0x3a3], ecx
	rol [0xb65], 0x46
	rol [0xa04], 0xf9
	rol [0xcbd], 0xd7
	rol [0x295], 0xc7
	rol esi, [0xa64], 1
	rol rdx, [0xb75], 1
	rol cl, [0x490], 1
	rol si, [0x8f5], 1
	rol ecx, [0xa17], 1
	rol rsi, [0x3db], 1
	rol bl, [0x54f], cl
	rol ax, [0x92c], cl
	rol ebp, [0xf93], cl
	rol rcx, [0x9dc], cl
	rol bl, [0x73f], cx
	rol si, [0x5ca], cx
	rol ebx, [0xc1b], cx
	rol rbp, [0xad4], cx
	rol dl, [0x814], ecx
	rol cx, [0x5b4], ecx
	rol ebx, [0xb91], ecx
	rol rcx, [0x385], ecx
	rol dl, [0xd94], 0x31
	rol dx, [0xdba], 0x44
	rol esi, [0xfd9], 0x69
	rol rbp, [0xa3e], 0xfb
