default rel
	popcnt bp, di
	popcnt dx, dx
	popcnt edx, dword [0xe26]
	popcnt ebx, ebp
	popcnt rbx, rbx
	popcnt rsi, rdx
	popcnt r15w, r10w
	popcnt r11d, r11d
	popcnt r13, r10
	popcnt r19w, r17w
	popcnt r18d, r28d
	popcnt r17, r28
	popcnt dx, word [eax+1]
	popcnt ax, word [eax+64]
	popcnt esi, dword [eax+1]
	popcnt esi, dword [eax+64]
	popcnt rdi, qword [eax+1]
	popcnt rdi, qword [eax+64]
	popcnt si, [0xebb]
	popcnt eax, [0x8fa]
	popcnt rsi, [0x217]
