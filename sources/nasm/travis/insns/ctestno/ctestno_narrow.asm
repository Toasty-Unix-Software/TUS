	ctestno 14, bl, dl
	ctestno 12, byte [0x172], al
	ctestno 10, si, dx
	ctestno 3, word [0x106], si
	ctestno 6, edx, eax
	ctestno 11, dword [0xc8e], esi

%ifdef ERROR
	ctestno 4, qword [0x1f6], rbp
	ctestno 2, rcx, rsi
	ctestno 2, r11b, r10b
	ctestno 9, r10w, r13w
	ctestno 0, r10d, r12d
	ctestno 7, r15, r12
	ctestno 9, r22b, r25b
	ctestno 2, r29w, r25w
	ctestno 6, r26d, r20d
	ctestno 5, r25, r21
	ctestno 13, qword [eax+1], rsi
	ctestno 15, qword [eax+64], rcx
	ctestno 14, qword [eax+1], -237830615
	ctestno 3, qword [eax+64], -497579771
	ctestno 3, [0x3d6], rax
	ctestno 11, [0xe4b], 280832863
%endif
