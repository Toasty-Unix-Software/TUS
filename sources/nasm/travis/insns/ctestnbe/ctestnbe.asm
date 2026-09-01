default rel
	ctestnbe 0, dl, al
	ctestnbe 4, bl, dl
	ctestnbe 13, cx, di
	ctestnbe 8, word [0xab8], ax
	ctestnbe 7, dword [0x870], ebp
	ctestnbe 3, ecx, esi
	ctestnbe 2, qword [0xe3d], rdx
	ctestnbe 4, rdx, rbp
	ctestnbe 13, r11b, r8b
	ctestnbe 13, r11w, r14w
	ctestnbe 12, r11d, r9d
	ctestnbe 11, r12, r13
	ctestnbe 14, r17b, r28b
	ctestnbe 2, r26w, r17w
	ctestnbe 5, r18d, r18d
	ctestnbe 1, r31, r25
	ctestnbe 14, byte [eax+1], dl
	ctestnbe 1, byte [eax+64], cl
	ctestnbe 8, word [eax+1], dx
	ctestnbe 4, word [eax+64], dx
	ctestnbe 3, dword [eax+1], eax
	ctestnbe 10, dword [eax+64], edx
	ctestnbe 3, qword [eax+1], rbx
	ctestnbe 2, qword [eax+64], rdx
	ctestnbe 3, byte [eax+1], 0x8d
	ctestnbe 3, byte [eax+64], 0x5d
	ctestnbe 11, word [eax+1], 0x52f1
	ctestnbe 10, word [eax+64], 0x84fa
	ctestnbe 8, dword [eax+1], 0xea3e431
	ctestnbe 13, dword [eax+64], 0xa14ea28
	ctestnbe 15, qword [eax+1], 210209992
	ctestnbe 15, qword [eax+64], 178177797
	ctestnbe 15, [0x7db], cl
	ctestnbe 7, [0xc63], cx
	ctestnbe 2, [0x519], ebx
	ctestnbe 0, [0x4fa], rsi
	ctestnbe 12, [0x46f], 0x81
	ctestnbe 5, [0xdbf], 0xf582
	ctestnbe 10, [0xa40], 0x7b5160e
	ctestnbe 2, [0xd60], 23010373
