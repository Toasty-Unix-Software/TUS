	ctestf 3, byte [0xf94], dl
	ctestf 14, cl, al
	ctestf 14, bx, bp
	ctestf 11, word [0xcea], bp
	ctestf 4, edx, esi
	ctestf 12, ebp, eax

%ifdef ERROR
	ctestf 11, qword [0x5ee], rdx
	ctestf 10, rcx, rsi
	ctestf 12, r15b, r8b
	ctestf 5, r13w, r11w
	ctestf 10, r8d, r8d
	ctestf 7, r8, r15
	ctestf 5, r26b, r23b
	ctestf 0, r28w, r16w
	ctestf 9, r21d, r19d
	ctestf 0, r16, r31
	ctestf 2, qword [eax+1], rdx
	ctestf 3, qword [eax+64], rax
	ctestf 6, qword [eax+1], -134059472
	ctestf 15, qword [eax+64], 473355415
	ctestf 8, [0xf03], rbp
	ctestf 13, [0xa10], 27245153
%endif
