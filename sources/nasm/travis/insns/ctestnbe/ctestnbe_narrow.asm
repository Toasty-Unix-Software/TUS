	ctestnbe 0, dl, al
	ctestnbe 4, bl, dl
	ctestnbe 13, cx, di
	ctestnbe 8, word [0xab8], ax
	ctestnbe 7, dword [0x870], ebp
	ctestnbe 3, ecx, esi

%ifdef ERROR
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
	ctestnbe 3, qword [eax+1], rbx
	ctestnbe 2, qword [eax+64], rdx
	ctestnbe 15, qword [eax+1], 210209992
	ctestnbe 15, qword [eax+64], 178177797
	ctestnbe 0, [0x4fa], rsi
	ctestnbe 2, [0xd60], 23010373
%endif
