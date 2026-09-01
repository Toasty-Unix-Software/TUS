default rel
	ccmp 6, byte [0xe6d], dl
	ccmp 9, byte [0xb44], dl
	ccmp 6, si, dx
	ccmp 1, word [0x7aa], ax
	ccmp 3, ecx, ebp
	ccmp 2, dword [0xf62], esi
	ccmp 13, qword [0xbe1], rbx
	ccmp 15, rdx, rbx
	ccmp 10, r11b, r13b
	ccmp 3, r13w, r13w
	ccmp 0, r11d, r9d
	ccmp 0, r15, r8
	ccmp 0, r17b, r16b
	ccmp 9, r27w, r19w
	ccmp 6, r24d, r28d
	ccmp 1, r27, r22
	ccmp 4, byte [eax+1], bl
	ccmp 0, byte [eax+64], al
	ccmp 8, word [eax+1], si
	ccmp 4, word [eax+64], bx
	ccmp 11, dword [eax+1], edx
	ccmp 2, dword [eax+64], ebx
	ccmp 7, qword [eax+1], rax
	ccmp 3, qword [eax+64], rbx
	ccmp 12, bl, byte [eax+1]
	ccmp 5, cl, byte [eax+64]
	ccmp 11, bx, word [eax+1]
	ccmp 15, bx, word [eax+64]
	ccmp 2, ebp, dword [eax+1]
	ccmp 6, edx, dword [eax+64]
	ccmp 10, rdi, qword [eax+1]
	ccmp 9, rcx, qword [eax+64]
	ccmp 11, word [eax+1], -108
	ccmp 15, word [eax+64], 23
	ccmp 4, dword [eax+1], 111
	ccmp 9, dword [eax+64], 64
	ccmp 4, qword [eax+1], 125
	ccmp 9, qword [eax+64], -1
	ccmp 5, byte [eax+1], 0xdc
	ccmp 4, byte [eax+64], 0xe9
	ccmp 13, word [eax+1], 0x9e12
	ccmp 4, word [eax+64], 0xfb87
	ccmp 0, dword [eax+1], 0x3e3b33d3
	ccmp 4, dword [eax+64], 0xe02a7f7
	ccmp 1, qword [eax+1], 350346
	ccmp 10, qword [eax+64], -222208413
	ccmp 6, [0x53e], dl
	ccmp 9, [0xa6e], ax
	ccmp 15, [0x800], esi
	ccmp 6, [0xa23], rax
	ccmp 10, bl, [0x382]
	ccmp 2, dx, [0x7e4]
	ccmp 14, ebx, [0x4b2]
	ccmp 14, rdx, [0x16a]
	ccmp 6, [0xe1e], 15
	ccmp 4, [0xd4d], -59
	ccmp 9, [0x9e3], 8
	ccmp 8, [0xdbf], 0xf9
	ccmp 7, [0x89e], 0xd37b
	ccmp 7, [0xf32], 0x3e5700b
	ccmp 4, [0x28d], -393521394
