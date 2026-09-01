default rel
	ror byte [0x4d6], 1
	ror cl, 1
	ror dx, 1
	ror word [0x9ce], 1
	ror dword [0xac7], 1
	ror ebp, 1
	ror qword [0xd84], 1
	ror rbp, 1
	ror r12b, 1
	ror r11w, 1
	ror r13d, 1
	ror r14, 1
	ror r27b, 1
	ror r17w, 1
	ror r25d, 1
	ror r30, 1
	ror byte [eax+1], 1
	ror byte [eax+64], 1
	ror word [eax+1], 1
	ror word [eax+64], 1
	ror dword [eax+1], 1
	ror dword [eax+64], 1
	ror qword [eax+1], 1
	ror qword [eax+64], 1
	ror byte [eax+1], cl
	ror byte [eax+64], cl
	ror word [eax+1], cl
	ror word [eax+64], cl
	ror dword [eax+1], cl
	ror dword [eax+64], cl
	ror qword [eax+1], cl
	ror qword [eax+64], cl
	ror byte [eax+1], cx
	ror byte [eax+64], cx
	ror word [eax+1], cx
	ror word [eax+64], cx
	ror dword [eax+1], cx
	ror dword [eax+64], cx
	ror qword [eax+1], cx
	ror qword [eax+64], cx
	ror byte [eax+1], ecx
	ror byte [eax+64], ecx
	ror word [eax+1], ecx
	ror word [eax+64], ecx
	ror dword [eax+1], ecx
	ror dword [eax+64], ecx
	ror qword [eax+1], ecx
	ror qword [eax+64], ecx
	ror byte [eax+1], 0xef
	ror byte [eax+64], 0x1
	ror word [eax+1], 0x5d
	ror word [eax+64], 0xad
	ror dword [eax+1], 0x2d
	ror dword [eax+64], 0x3b
	ror qword [eax+1], 0x65
	ror qword [eax+64], 0x63
	ror ecx, dword [eax+1], 0x41
	ror eax, dword [eax+64], 0x8a
	ror rdx, qword [eax+1], 0xbd
	ror rbx, qword [eax+64], 0x49
	ror bl, byte [eax+1], 1
	ror bl, byte [eax+64], 1
	ror bp, word [eax+1], 1
	ror cx, word [eax+64], 1
	ror ecx, dword [eax+1], 1
	ror edx, dword [eax+64], 1
	ror rax, qword [eax+1], 1
	ror rbx, qword [eax+64], 1
	ror cl, byte [eax+1], cl
	ror dl, byte [eax+64], cl
	ror bx, word [eax+1], cl
	ror bx, word [eax+64], cl
	ror esi, dword [eax+1], cl
	ror esi, dword [eax+64], cl
	ror rsi, qword [eax+1], cl
	ror rdi, qword [eax+64], cl
	ror cl, byte [eax+1], cx
	ror al, byte [eax+64], cx
	ror ax, word [eax+1], cx
	ror cx, word [eax+64], cx
	ror ebp, dword [eax+1], cx
	ror ebp, dword [eax+64], cx
	ror rdi, qword [eax+1], cx
	ror rdi, qword [eax+64], cx
	ror dl, byte [eax+1], ecx
	ror dl, byte [eax+64], ecx
	ror ax, word [eax+1], ecx
	ror di, word [eax+64], ecx
	ror ecx, dword [eax+1], ecx
	ror esi, dword [eax+64], ecx
	ror rbx, qword [eax+1], ecx
	ror rdx, qword [eax+64], ecx
	ror cl, byte [eax+1], 0x14
	ror cl, byte [eax+64], 0x41
	ror cx, word [eax+1], 0x78
	ror cx, word [eax+64], 0x7
	ror edi, dword [eax+1], 0xa2
	ror esi, dword [eax+64], 0x6c
	ror rbp, qword [eax+1], 0x76
	ror rsi, qword [eax+64], 0x5f
	ror [0x441], 1
	ror [0xd95], 1
	ror [0xf26], 1
	ror [0x293], 1
	ror [0x8df], cl
	ror [0xb21], cl
	ror [0x159], cl
	ror [0x569], cl
	ror [0x4b5], cx
	ror [0xa04], cx
	ror [0x835], cx
	ror [0xffb], cx
	ror [0x370], ecx
	ror [0xe94], ecx
	ror [0x95a], ecx
	ror [0x848], ecx
	ror [0x2ff], 0x8c
	ror [0x91c], 0xe6
	ror [0x770], 0xc0
	ror [0xd8c], 0x41
	ror edi, [0x2be], 0xdd
	ror rdi, [0x78b], 0xa5
	ror al, [0xf3f], 1
	ror si, [0xf9b], 1
	ror ecx, [0xf70], 1
	ror rdx, [0x5ed], 1
	ror dl, [0x483], cl
	ror cx, [0x591], cl
	ror edx, [0xee6], cl
	ror rdi, [0x2a9], cl
	ror al, [0x6ac], cx
	ror dx, [0x2ba], cx
	ror ebp, [0xa22], cx
	ror rbx, [0x363], cx
	ror dl, [0x9dc], ecx
	ror si, [0x945], ecx
	ror ebp, [0xd8e], ecx
	ror rbp, [0x749], ecx
	ror dl, [0x711], 0x35
	ror si, [0xe8b], 0x3c
	ror ebx, [0x3d5], 0xc8
	ror rcx, [0xaef], 0xdd
