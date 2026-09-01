default rel
	ctests 12, byte [0x30d], al
	ctests 11, bl, dl
	ctests 0, dx, ax
	ctests 9, bx, bp
	ctests 4, dword [0x626], edx
	ctests 11, dword [0xdd1], edi
	ctests 14, rcx, rcx
	ctests 11, qword [0x25f], rcx
	ctests 5, r13b, r14b
	ctests 6, r13w, r8w
	ctests 7, r10d, r11d
	ctests 5, r12, r9
	ctests 11, r19b, r27b
	ctests 3, r29w, r27w
	ctests 5, r25d, r16d
	ctests 14, r24, r21
	ctests 13, byte [eax+1], cl
	ctests 9, byte [eax+64], bl
	ctests 13, word [eax+1], bp
	ctests 11, word [eax+64], bx
	ctests 6, dword [eax+1], ecx
	ctests 13, dword [eax+64], eax
	ctests 12, qword [eax+1], rax
	ctests 13, qword [eax+64], rcx
	ctests 15, byte [eax+1], 0xa8
	ctests 7, byte [eax+64], 0x2c
	ctests 11, word [eax+1], 0xdc87
	ctests 3, word [eax+64], 0x6b3d
	ctests 14, dword [eax+1], 0x7bf8da2
	ctests 13, dword [eax+64], 0x3064c992
	ctests 0, qword [eax+1], 505883947
	ctests 10, qword [eax+64], 178059430
	ctests 1, [0xb26], cl
	ctests 12, [0x354], di
	ctests 12, [0xbe4], edi
	ctests 8, [0xcfe], rbx
	ctests 3, [0xcb8], 0x8b
	ctests 12, [0xf7a], 0xe9d3
	ctests 11, [0x255], 0x1dc50c27
	ctests 13, [0x2ae], -85772268
