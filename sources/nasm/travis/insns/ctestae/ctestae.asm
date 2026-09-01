default rel
	ctestae 6, byte [0x5f6], dl
	ctestae 6, bl, cl
	ctestae 6, bp, di
	ctestae 3, cx, cx
	ctestae 10, edi, ebx
	ctestae 7, ecx, edi
	ctestae 8, qword [0xd7b], rbp
	ctestae 6, qword [0x27c], rbx
	ctestae 10, r13b, r10b
	ctestae 1, r11w, r12w
	ctestae 4, r15d, r11d
	ctestae 6, r13, r9
	ctestae 12, r26b, r19b
	ctestae 1, r26w, r22w
	ctestae 0, r21d, r24d
	ctestae 6, r20, r19
	ctestae 7, byte [eax+1], al
	ctestae 6, byte [eax+64], cl
	ctestae 6, word [eax+1], cx
	ctestae 0, word [eax+64], dx
	ctestae 7, dword [eax+1], edx
	ctestae 9, dword [eax+64], ecx
	ctestae 13, qword [eax+1], rdi
	ctestae 0, qword [eax+64], rbx
	ctestae 12, byte [eax+1], 0xab
	ctestae 10, byte [eax+64], 0xaf
	ctestae 7, word [eax+1], 0x8015
	ctestae 2, word [eax+64], 0xe6cf
	ctestae 9, dword [eax+1], 0x115d6891
	ctestae 13, dword [eax+64], 0x37816074
	ctestae 10, qword [eax+1], -11623662
	ctestae 11, qword [eax+64], 189146921
	ctestae 6, [0x29b], cl
	ctestae 11, [0xb3a], bx
	ctestae 2, [0x60f], edx
	ctestae 15, [0x492], rdx
	ctestae 14, [0xd9f], 0xaa
	ctestae 0, [0x1d8], 0xdddd
	ctestae 12, [0xbae], 0x35bdc2dc
	ctestae 12, [0x3cc], 287911890
