default rel
	ccmpng 14, cl, bl
	ccmpng 2, byte [0x5e9], dl
	ccmpng 12, bx, dx
	ccmpng 8, di, dx
	ccmpng 2, eax, ebx
	ccmpng 6, ebx, edx
	ccmpng 13, qword [0x635], rcx
	ccmpng 14, qword [0x725], rdi
	ccmpng 6, r9b, r15b
	ccmpng 1, r9w, r15w
	ccmpng 6, r8d, r9d
	ccmpng 6, r14, r12
	ccmpng 8, r29b, r22b
	ccmpng 14, r27w, r23w
	ccmpng 7, r16d, r23d
	ccmpng 13, r29, r19
	ccmpng 13, byte [eax+1], cl
	ccmpng 2, byte [eax+64], dl
	ccmpng 9, word [eax+1], ax
	ccmpng 4, word [eax+64], ax
	ccmpng 0, dword [eax+1], ebp
	ccmpng 15, dword [eax+64], ecx
	ccmpng 0, qword [eax+1], rdx
	ccmpng 15, qword [eax+64], rbp
	ccmpng 14, cl, byte [eax+1]
	ccmpng 0, cl, byte [eax+64]
	ccmpng 3, ax, word [eax+1]
	ccmpng 1, bx, word [eax+64]
	ccmpng 8, ebx, dword [eax+1]
	ccmpng 12, ebx, dword [eax+64]
	ccmpng 11, rbp, qword [eax+1]
	ccmpng 2, rbx, qword [eax+64]
	ccmpng 4, word [eax+1], 125
	ccmpng 11, word [eax+64], -79
	ccmpng 8, dword [eax+1], -63
	ccmpng 9, dword [eax+64], 123
	ccmpng 9, qword [eax+1], -114
	ccmpng 2, qword [eax+64], -59
	ccmpng 11, byte [eax+1], 0x8e
	ccmpng 11, byte [eax+64], 0xc
	ccmpng 7, word [eax+1], 0x816a
	ccmpng 15, word [eax+64], 0x7c6b
	ccmpng 5, dword [eax+1], 0x19897244
	ccmpng 3, dword [eax+64], 0x1f32ad39
	ccmpng 6, qword [eax+1], -178735910
	ccmpng 14, qword [eax+64], 39042579
	ccmpng 9, [0x167], al
	ccmpng 9, [0x635], di
	ccmpng 9, [0xe9c], edi
	ccmpng 7, [0x484], rdx
	ccmpng 4, al, [0x954]
	ccmpng 6, cx, [0x5a0]
	ccmpng 6, ecx, [0xf1e]
	ccmpng 5, rax, [0xd61]
	ccmpng 5, [0xcb5], 68
	ccmpng 8, [0xe79], -113
	ccmpng 4, [0xa6b], -47
	ccmpng 7, [0x197], 0xb3
	ccmpng 5, [0xf2f], 0x1ad8
	ccmpng 13, [0xd9e], 0x5a67ca3
	ccmpng 3, [0x6f3], 440372382
