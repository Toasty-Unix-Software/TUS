default rel
	movsxw eax, ax
	movsxw eax, ax
	movsxw ax, cx
	movsxw cx, si
	movsxw edi, word [0x190]
	movsxw esi, ax
	movsxw rdi, word [0xef4]
	movsxw rbp, dx
	movsxw r8w, r8w
	movsxw r11d, r13w
	movsxw r11, r9w
	movsxw r17w, r23w
	movsxw r16d, r18w
	movsxw r29, r28w
	movsxw bx, word [eax+1]
	movsxw dx, word [eax+64]
	movsxw ebp, word [eax+1]
	movsxw ecx, word [eax+64]
	movsxw rsi, word [eax+1]
	movsxw rbp, word [eax+64]
	movsxw ax, [0x672]
	movsxw ecx, [0xf58]
	movsxw rcx, [0x32d]
