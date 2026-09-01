	lzcnt bp, bp
	lzcnt bp, cx
	lzcnt ebx, edi
	lzcnt ebp, ebx

%ifdef ERROR
	lzcnt rdx, rbp
	lzcnt rdi, qword [0xed6]
	lzcnt r11w, r9w
	lzcnt r12d, r9d
	lzcnt r14, r14
	lzcnt r17w, r27w
	lzcnt r17d, r21d
	lzcnt r20, r31
	lzcnt rax, qword [eax+1]
	lzcnt rdx, qword [eax+64]
	lzcnt rdx, [0x306]
%endif
