default rel
	sbb byte [0x152], al
	sbb byte [0xc9c], dl
	sbb cx, bx
	sbb cx, si
	sbb edx, ebx
	sbb esi, ebp
	sbb qword [0xd3b], rcx
	sbb qword [0x514], rax
	sbb r9b, r9b
	sbb r15w, r10w
	sbb r14d, r11d
	sbb r9, r12
	sbb r23b, r16b
	sbb r17w, r24w
	sbb r21d, r20d
	sbb r28, r31
	sbb byte [eax+1], dl
	sbb byte [eax+64], bl
	sbb word [eax+1], dx
	sbb word [eax+64], si
	sbb dword [eax+1], eax
	sbb dword [eax+64], edi
	sbb qword [eax+1], rdi
	sbb qword [eax+64], rdx
	sbb cl, byte [eax+1]
	sbb al, byte [eax+64]
	sbb ax, word [eax+1]
	sbb cx, word [eax+64]
	sbb eax, dword [eax+1]
	sbb ecx, dword [eax+64]
	sbb rsi, qword [eax+1]
	sbb rsi, qword [eax+64]
	sbb byte [eax+1], 0xd8
	sbb byte [eax+64], 0xd8
	sbb word [eax+1], 20
	sbb word [eax+64], -96
	sbb word [eax+1], 0x588f
	sbb word [eax+64], 0xea0b
	sbb dword [eax+1], -14
	sbb dword [eax+64], 123
	sbb dword [eax+1], 0x120b9d95
	sbb dword [eax+64], 0x169bbc40
	sbb qword [eax+1], -113
	sbb qword [eax+64], 128
	sbb qword [eax+1], 138481480
	sbb qword [eax+64], 479373787
	sbb bl, dl, byte [eax+1]
	sbb dl, al, byte [eax+64]
	sbb ax, si, word [eax+1]
	sbb bp, cx, word [eax+64]
	sbb eax, ebp, dword [eax+1]
	sbb ebx, ecx, dword [eax+64]
	sbb rax, rbp, qword [eax+1]
	sbb rcx, rax, qword [eax+64]
	sbb bl, byte [eax+1], dl
	sbb bl, byte [eax+64], cl
	sbb ax, word [eax+1], bp
	sbb dx, word [eax+64], si
	sbb ebp, dword [eax+1], ebp
	sbb ecx, dword [eax+64], eax
	sbb rax, qword [eax+1], rdx
	sbb rdx, qword [eax+64], rsi
	sbb bp, word [eax+1], -16
	sbb bx, word [eax+64], -14
	sbb edi, dword [eax+1], -13
	sbb edi, dword [eax+64], 73
	sbb rcx, qword [eax+1], 31
	sbb rbx, qword [eax+64], -22
	sbb dl, byte [eax+1], 0xc0
	sbb cl, byte [eax+64], 0x68
	sbb ax, word [eax+1], 0x322d
	sbb bp, word [eax+64], 0x88dd
	sbb esi, dword [eax+1], 0x21b26c61
	sbb esi, dword [eax+64], 0x2118f2f7
	sbb rcx, qword [eax+1], -304481604
	sbb rdx, qword [eax+64], 31411479
	sbb [0x2d1], bl
	sbb [0x56f], bx
	sbb [0x43c], ecx
	sbb [0xf6d], rsi
	sbb dl, [0x780]
	sbb si, [0x5f2]
	sbb esi, [0xc77]
	sbb rax, [0x5e8]
	sbb [0xb9a], 0xcb
	sbb [0x550], -59
	sbb [0xaca], 0xd527
	sbb [0xed3], 59
	sbb [0xf09], 0x1b457a6d
	sbb [0xd6f], 111
	sbb [0xeed], 503962257
	sbb cl, al, [0xadd]
	sbb bx, ax, [0xd25]
	sbb edx, eax, [0xc1f]
	sbb rdx, rbp, [0x995]
	sbb dl, [0xf4b], dl
	sbb di, [0xc7c], dx
	sbb ecx, [0x149], eax
	sbb rdi, [0x541], rsi
	sbb bp, [0xb60], -122
	sbb ecx, [0xab1], -72
	sbb rbp, [0x719], -21
	sbb al, [0x293], 0xc9
	sbb ax, [0x48c], 0xc298
	sbb eax, [0xa8d], 0x1809bbdb
	sbb rax, [0x3cc], 363984859
