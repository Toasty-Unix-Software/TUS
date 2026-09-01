default rel
	test cl, dl
	test cl, al
	test dx, si
	test dx, di
	test dword [0x934], edx
	test esi, ebp
	test qword [0x9e0], rdi
	test rax, rax
	test r14b, r15b
	test r10w, r8w
	test r12d, r14d
	test r11, r8
	test r23b, r27b
	test r22w, r28w
	test r17d, r23d
	test r28, r16
	test byte [eax+1], bl
	test byte [eax+64], bl
	test word [eax+1], bp
	test word [eax+64], dx
	test dword [eax+1], ebp
	test dword [eax+64], edi
	test qword [eax+1], rdi
	test qword [eax+64], rax
	test dl, byte [eax+1]
	test al, byte [eax+64]
	test di, word [eax+1]
	test di, word [eax+64]
	test esi, dword [eax+1]
	test ebp, dword [eax+64]
	test rdx, qword [eax+1]
	test rdx, qword [eax+64]
	test byte [eax+1], 0x3c
	test byte [eax+64], 0x26
	test word [eax+1], 0xd787
	test word [eax+64], 0xea90
	test dword [eax+1], 0x1587e7cf
	test dword [eax+64], 0x6bd7579
	test qword [eax+1], 174016346
	test qword [eax+64], -197546387
	test [0xbcf], bl
	test [0xdd0], bp
	test [0x280], edi
	test [0x6a9], rbp
	test bl, [0x650]
	test si, [0x56f]
	test ebx, [0xf07]
	test rsi, [0xa80]
	test [0xc0a], 0xd4
	test [0x9ab], 0xae70
	test [0xbe8], 0x1f842620
	test [0xd04], -6445748
