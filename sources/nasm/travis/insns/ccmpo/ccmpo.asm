default rel
	ccmpo 15, bl, cl
	ccmpo 3, al, al
	ccmpo 3, cx, si
	ccmpo 9, ax, bx
	ccmpo 11, dword [0x2c9], edi
	ccmpo 15, dword [0x7c2], edi
	ccmpo 14, rcx, rsi
	ccmpo 3, qword [0x920], rbp
	ccmpo 9, r9b, r10b
	ccmpo 13, r13w, r11w
	ccmpo 10, r11d, r11d
	ccmpo 4, r10, r13
	ccmpo 1, r23b, r24b
	ccmpo 0, r26w, r23w
	ccmpo 7, r30d, r21d
	ccmpo 12, r17, r24
	ccmpo 1, byte [eax+1], bl
	ccmpo 12, byte [eax+64], al
	ccmpo 6, word [eax+1], cx
	ccmpo 1, word [eax+64], bx
	ccmpo 6, dword [eax+1], edx
	ccmpo 0, dword [eax+64], ebx
	ccmpo 8, qword [eax+1], rax
	ccmpo 1, qword [eax+64], rax
	ccmpo 15, cl, byte [eax+1]
	ccmpo 11, al, byte [eax+64]
	ccmpo 3, cx, word [eax+1]
	ccmpo 9, cx, word [eax+64]
	ccmpo 4, ebp, dword [eax+1]
	ccmpo 9, edi, dword [eax+64]
	ccmpo 2, rsi, qword [eax+1]
	ccmpo 5, rax, qword [eax+64]
	ccmpo 1, word [eax+1], 95
	ccmpo 6, word [eax+64], -68
	ccmpo 15, dword [eax+1], 30
	ccmpo 7, dword [eax+64], 128
	ccmpo 0, qword [eax+1], -25
	ccmpo 5, qword [eax+64], -77
	ccmpo 0, byte [eax+1], 0xb7
	ccmpo 5, byte [eax+64], 0x81
	ccmpo 2, word [eax+1], 0x39f9
	ccmpo 2, word [eax+64], 0xba5a
	ccmpo 1, dword [eax+1], 0x225e0831
	ccmpo 2, dword [eax+64], 0xd0d0c19
	ccmpo 15, qword [eax+1], 439212364
	ccmpo 8, qword [eax+64], 125419640
	ccmpo 12, [0x156], dl
	ccmpo 14, [0xeab], bp
	ccmpo 7, [0x3d3], eax
	ccmpo 13, [0x5f7], rax
	ccmpo 4, al, [0x89e]
	ccmpo 5, si, [0x20c]
	ccmpo 0, ebx, [0x930]
	ccmpo 4, rbp, [0xe09]
	ccmpo 2, [0x6a3], -4
	ccmpo 5, [0xe6f], -40
	ccmpo 8, [0xb31], -90
	ccmpo 15, [0xe98], 0xb2
	ccmpo 15, [0xb6d], 0x124c
	ccmpo 8, [0x68f], 0x1adfaefb
	ccmpo 3, [0x443], 17233092
