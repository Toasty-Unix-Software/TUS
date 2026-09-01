default rel
	ccmpge 7, bl, bl
	ccmpge 4, bl, al
	ccmpge 13, ax, dx
	ccmpge 10, bx, dx
	ccmpge 11, edx, edx
	ccmpge 1, dword [0xdde], edi
	ccmpge 4, rbx, rbx
	ccmpge 15, qword [0xb98], rdx
	ccmpge 12, r11b, r12b
	ccmpge 11, r12w, r8w
	ccmpge 6, r8d, r12d
	ccmpge 1, r13, r13
	ccmpge 9, r20b, r21b
	ccmpge 9, r18w, r18w
	ccmpge 5, r17d, r18d
	ccmpge 7, r26, r18
	ccmpge 4, byte [eax+1], cl
	ccmpge 10, byte [eax+64], al
	ccmpge 9, word [eax+1], bp
	ccmpge 10, word [eax+64], bx
	ccmpge 2, dword [eax+1], edi
	ccmpge 14, dword [eax+64], edi
	ccmpge 0, qword [eax+1], rax
	ccmpge 11, qword [eax+64], rbx
	ccmpge 14, dl, byte [eax+1]
	ccmpge 10, al, byte [eax+64]
	ccmpge 5, dx, word [eax+1]
	ccmpge 0, ax, word [eax+64]
	ccmpge 7, eax, dword [eax+1]
	ccmpge 10, edx, dword [eax+64]
	ccmpge 11, rdi, qword [eax+1]
	ccmpge 1, rdx, qword [eax+64]
	ccmpge 10, word [eax+1], -22
	ccmpge 6, word [eax+64], 24
	ccmpge 14, dword [eax+1], -70
	ccmpge 1, dword [eax+64], 90
	ccmpge 14, qword [eax+1], 100
	ccmpge 12, qword [eax+64], 46
	ccmpge 9, byte [eax+1], 0xc9
	ccmpge 3, byte [eax+64], 0x2f
	ccmpge 2, word [eax+1], 0x7b4d
	ccmpge 15, word [eax+64], 0x6d6d
	ccmpge 2, dword [eax+1], 0x1d009ee0
	ccmpge 5, dword [eax+64], 0x29ab7d5e
	ccmpge 1, qword [eax+1], 303069272
	ccmpge 7, qword [eax+64], 184283815
	ccmpge 0, [0xdc9], cl
	ccmpge 4, [0xf24], bp
	ccmpge 11, [0xd36], ebx
	ccmpge 11, [0x692], rax
	ccmpge 9, al, [0xfd2]
	ccmpge 11, bp, [0x381]
	ccmpge 5, ecx, [0x7a5]
	ccmpge 11, rcx, [0xbd9]
	ccmpge 9, [0x947], -43
	ccmpge 7, [0x94e], -40
	ccmpge 9, [0x178], 38
	ccmpge 10, [0x344], 0xa2
	ccmpge 15, [0x898], 0xd7a0
	ccmpge 4, [0x728], 0x45bd2ae
	ccmpge 15, [0x7cf], 86009347
