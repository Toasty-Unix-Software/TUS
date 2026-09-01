default rel
	ctestle 7, dl, al
	ctestle 4, al, bl
	ctestle 2, bx, di
	ctestle 12, bp, bx
	ctestle 14, ebp, ecx
	ctestle 15, ebx, edx
	ctestle 11, rbp, rcx
	ctestle 12, rax, rax
	ctestle 10, r14b, r13b
	ctestle 11, r8w, r13w
	ctestle 4, r10d, r11d
	ctestle 6, r13, r15
	ctestle 13, r23b, r18b
	ctestle 5, r19w, r26w
	ctestle 13, r18d, r27d
	ctestle 5, r30, r29
	ctestle 14, byte [eax+1], cl
	ctestle 0, byte [eax+64], cl
	ctestle 11, word [eax+1], ax
	ctestle 3, word [eax+64], si
	ctestle 2, dword [eax+1], edi
	ctestle 12, dword [eax+64], edx
	ctestle 9, qword [eax+1], rax
	ctestle 0, qword [eax+64], rsi
	ctestle 10, byte [eax+1], 0xa0
	ctestle 0, byte [eax+64], 0x8e
	ctestle 1, word [eax+1], 0x213d
	ctestle 9, word [eax+64], 0x9980
	ctestle 12, dword [eax+1], 0x3ebcb1e0
	ctestle 14, dword [eax+64], 0x1060ecac
	ctestle 12, qword [eax+1], -478784017
	ctestle 12, qword [eax+64], -45324884
	ctestle 11, [0xfaf], dl
	ctestle 9, [0xe31], bx
	ctestle 3, [0xe98], eax
	ctestle 8, [0xe3f], rax
	ctestle 14, [0x97f], 0x15
	ctestle 4, [0x403], 0x95a1
	ctestle 5, [0xa88], 0x3972715f
	ctestle 7, [0x70b], 238658793
