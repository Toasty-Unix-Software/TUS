default rel
	movbe bx, word [0xdea]
	movbe dx, word [0xade]
	movbe ecx, dword [0xcad]
	movbe eax, dword [0x4b5]
	movbe rbp, qword [0xb81]
	movbe rdx, qword [0x7f8]
	movbe word [0xc58], ax
	movbe word [0xb12], bp
	movbe r11w, word [0xd51]
	movbe r11d, dword [0xfe9]
	movbe r9, qword [0xcea]
	movbe word [0x708], r15w
	movbe r25w, word [0xa9a]
	movbe r29d, dword [0x221]
	movbe r29, qword [0x4a5]
	movbe word [0x723], r19w
	movbe si, word [eax+1]
	movbe di, word [eax+64]
	movbe esi, dword [eax+1]
	movbe ecx, dword [eax+64]
	movbe rbx, qword [eax+1]
	movbe rdi, qword [eax+64]
	movbe word [eax+1], di
	movbe word [eax+64], bx
	movbe dword [eax+1], eax
	movbe dword [eax+64], esi
	movbe qword [eax+1], rbp
	movbe qword [eax+64], rcx
	movbe si, [0x960]
	movbe edx, [0x694]
	movbe rdx, [0x3b1]
	movbe [0x671], di
	movbe [0xdbe], ebp
	movbe [0xb68], rcx
