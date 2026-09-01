default rel
	bsr di, dx
	bsr cx, word [0x696]
	bsr edx, ecx
	bsr ebx, esi
	bsr rbx, rdx
	bsr rcx, qword [0x900]
	bsr r9w, r9w
	bsr r12d, r11d
	bsr r10, r15
	bsr r24w, r28w
	bsr r23d, r21d
	bsr r31, r21
	bsr ax, word [eax+1]
	bsr si, word [eax+64]
	bsr edx, dword [eax+1]
	bsr edi, dword [eax+64]
	bsr rdx, qword [eax+1]
	bsr rbx, qword [eax+64]
	bsr bx, [0x6ec]
	bsr edi, [0x8a2]
	bsr rdx, [0x450]
