default rel
	cmovb ax, word [0x81e]
	cmovb bp, di
	cmovb esi, dword [0xe43]
	cmovb ebx, ecx
	cmovb rdi, rdx
	cmovb rbp, rcx
	cmovb dx, bp, ax
	cmovb cx, di, cx
	cmovb r13w, r8w
	cmovb r13d, r9d
	cmovb r13, r10
	cmovb r12w, r12w, r10w
	cmovb r24w, r23w
	cmovb r23d, r20d
	cmovb r28, r19
	cmovb r24w, r26w, r29w
	cmovb si, word [eax+1]
	cmovb cx, word [eax+64]
	cmovb eax, dword [eax+1]
	cmovb esi, dword [eax+64]
	cmovb rbx, qword [eax+1]
	cmovb rdx, qword [eax+64]
	cmovb di, bx, word [eax+1]
	cmovb bp, ax, word [eax+64]
	cmovb eax, edx, dword [eax+1]
	cmovb ecx, ebp, dword [eax+64]
	cmovb rcx, rdx, qword [eax+1]
	cmovb rsi, rbp, qword [eax+64]
	cmovb cx, [0xf9f]
	cmovb edi, [0x7ec]
	cmovb rdx, [0x415]
	cmovb cx, cx, [0xbe9]
	cmovb ebx, esi, [0x7de]
	cmovb rdi, rsi, [0xb3f]
