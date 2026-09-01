default rel
	ctesta 14, dl, al
	ctesta 4, bl, dl
	ctesta 6, word [0xf47], cx
	ctesta 15, word [0x61d], ax
	ctesta 1, ecx, eax
	ctesta 14, edi, edi
	ctesta 6, rdx, rsi
	ctesta 10, rdi, rbp
	ctesta 3, r11b, r11b
	ctesta 12, r12w, r12w
	ctesta 1, r13d, r13d
	ctesta 7, r13, r10
	ctesta 4, r28b, r17b
	ctesta 9, r24w, r23w
	ctesta 14, r24d, r28d
	ctesta 9, r31, r21
	ctesta 3, byte [eax+1], cl
	ctesta 5, byte [eax+64], cl
	ctesta 6, word [eax+1], si
	ctesta 7, word [eax+64], dx
	ctesta 2, dword [eax+1], eax
	ctesta 10, dword [eax+64], edx
	ctesta 7, qword [eax+1], rbp
	ctesta 11, qword [eax+64], rax
	ctesta 6, byte [eax+1], 0xf
	ctesta 9, byte [eax+64], 0xf9
	ctesta 2, word [eax+1], 0x38bb
	ctesta 15, word [eax+64], 0xb287
	ctesta 8, dword [eax+1], 0x6f40208
	ctesta 4, dword [eax+64], 0x26b8ae06
	ctesta 7, qword [eax+1], -514777320
	ctesta 13, qword [eax+64], 279081498
	ctesta 8, [0xa4f], dl
	ctesta 9, [0xa77], ax
	ctesta 14, [0x152], ecx
	ctesta 13, [0x5d0], rbp
	ctesta 0, [0xd10], 0xd3
	ctesta 8, [0xe9e], 0xbade
	ctesta 5, [0xfe1], 0x1f8a3ed8
	ctesta 4, [0xf82], 307270225
