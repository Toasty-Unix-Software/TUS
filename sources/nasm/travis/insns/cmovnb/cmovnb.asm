default rel
	cmovnb bp, si
	cmovnb bp, bx
	cmovnb ecx, dword [0xb39]
	cmovnb ebp, eax
	cmovnb rcx, rdx
	cmovnb rsi, rcx
	cmovnb dx, cx, word [0xebd]
	cmovnb bx, si, cx
	cmovnb r11w, r10w
	cmovnb r10d, r12d
	cmovnb r8, r9
	cmovnb r10w, r12w, r10w
	cmovnb r18w, r20w
	cmovnb r30d, r27d
	cmovnb r29, r30
	cmovnb r20w, r31w, r23w
	cmovnb si, word [eax+1]
	cmovnb bx, word [eax+64]
	cmovnb eax, dword [eax+1]
	cmovnb ebp, dword [eax+64]
	cmovnb rbp, qword [eax+1]
	cmovnb rax, qword [eax+64]
	cmovnb ax, bp, word [eax+1]
	cmovnb si, bx, word [eax+64]
	cmovnb edx, edx, dword [eax+1]
	cmovnb eax, edx, dword [eax+64]
	cmovnb rcx, rbx, qword [eax+1]
	cmovnb rdx, rcx, qword [eax+64]
	cmovnb bp, [0x3f2]
	cmovnb eax, [0xbad]
	cmovnb rdi, [0x14c]
	cmovnb si, si, [0xf77]
	cmovnb edx, ebx, [0x833]
	cmovnb rdx, rdx, [0x992]
