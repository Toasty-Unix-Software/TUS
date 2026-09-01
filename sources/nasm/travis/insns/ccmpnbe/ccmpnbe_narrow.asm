	ccmpnbe 10, al, al
	ccmpnbe 10, byte [0xa24], al
	ccmpnbe 2, word [0x850], cx
	ccmpnbe 9, cx, bp
	ccmpnbe 5, edi, edi
	ccmpnbe 4, edx, eax

%ifdef ERROR
	ccmpnbe 4, rdx, rcx
	ccmpnbe 13, qword [0xcfd], rsi
	ccmpnbe 0, r8b, r11b
	ccmpnbe 3, r10w, r12w
	ccmpnbe 15, r12d, r13d
	ccmpnbe 9, r10, r12
	ccmpnbe 9, r23b, r28b
	ccmpnbe 11, r21w, r18w
	ccmpnbe 14, r30d, r29d
	ccmpnbe 2, r27, r22
	ccmpnbe 11, qword [eax+1], rax
	ccmpnbe 7, qword [eax+64], rdi
	ccmpnbe 6, rdi, qword [eax+1]
	ccmpnbe 3, rdi, qword [eax+64]
	ccmpnbe 11, qword [eax+1], 108
	ccmpnbe 7, qword [eax+64], -35
	ccmpnbe 2, qword [eax+1], -18119360
	ccmpnbe 9, qword [eax+64], -62441155
	ccmpnbe 15, [0x8bd], rdi
	ccmpnbe 10, rdi, [0x65d]
	ccmpnbe 1, [0xd93], -80
	ccmpnbe 7, [0x6b6], 246365479
%endif
