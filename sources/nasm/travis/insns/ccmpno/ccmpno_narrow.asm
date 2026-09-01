	ccmpno 0, dl, dl
	ccmpno 1, al, bl
	ccmpno 1, bx, ax
	ccmpno 8, word [0xf51], bp
	ccmpno 4, ecx, ecx
	ccmpno 12, edi, eax

%ifdef ERROR
	ccmpno 14, qword [0x98f], rsi
	ccmpno 9, qword [0x529], rax
	ccmpno 14, r12b, r10b
	ccmpno 14, r8w, r14w
	ccmpno 0, r15d, r13d
	ccmpno 9, r13, r8
	ccmpno 6, r28b, r29b
	ccmpno 8, r18w, r27w
	ccmpno 8, r17d, r17d
	ccmpno 12, r24, r22
	ccmpno 8, qword [eax+1], rbp
	ccmpno 0, qword [eax+64], rcx
	ccmpno 6, rax, qword [eax+1]
	ccmpno 15, rbx, qword [eax+64]
	ccmpno 14, qword [eax+1], -21
	ccmpno 13, qword [eax+64], -16
	ccmpno 12, qword [eax+1], -396866397
	ccmpno 14, qword [eax+64], -42467842
	ccmpno 10, [0xc0d], rbp
	ccmpno 3, rbx, [0x151]
	ccmpno 4, [0xa13], -71
	ccmpno 2, [0x3ea], -361838799
%endif
