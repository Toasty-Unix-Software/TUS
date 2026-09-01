default rel
	lzcnt bp, bp
	lzcnt bp, cx
	lzcnt ebx, edi
	lzcnt ebp, ebx
	lzcnt rdx, rbp
	lzcnt rdi, qword [0xed6]
	lzcnt r11w, r9w
	lzcnt r12d, r9d
	lzcnt r14, r14
	lzcnt r17w, r27w
	lzcnt r17d, r21d
	lzcnt r20, r31
	lzcnt si, word [eax+1]
	lzcnt ax, word [eax+64]
	lzcnt edi, dword [eax+1]
	lzcnt edx, dword [eax+64]
	lzcnt rax, qword [eax+1]
	lzcnt rdx, qword [eax+64]
	lzcnt si, [0xb38]
	lzcnt ecx, [0x31d]
	lzcnt rdx, [0x306]
