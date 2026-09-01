	ctesta 14, dl, al
	ctesta 4, bl, dl
	ctesta 6, word [0xf47], cx
	ctesta 15, word [0x61d], ax
	ctesta 1, ecx, eax
	ctesta 14, edi, edi

%ifdef ERROR
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
	ctesta 7, qword [eax+1], rbp
	ctesta 11, qword [eax+64], rax
	ctesta 7, qword [eax+1], -514777320
	ctesta 13, qword [eax+64], 279081498
	ctesta 13, [0x5d0], rbp
	ctesta 4, [0xf82], 307270225
%endif
