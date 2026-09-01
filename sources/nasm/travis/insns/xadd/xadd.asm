default rel
	xadd dl, al
	xadd cl, dl
	xadd ax, si
	xadd bp, bx
	xadd ebx, ebp
	xadd edx, ebp
	xadd qword [0xf00], rdi
	xadd rdx, rax
	xadd r11b, r13b
	xadd r12w, r10w
	xadd r15d, r13d
	xadd r14, r10
	xadd r24b, r30b
	xadd r18w, r16w
	xadd r29d, r20d
	xadd r27, r24
	xadd byte [eax+1], dl
	xadd byte [eax+64], bl
	xadd word [eax+1], bx
	xadd word [eax+64], ax
	xadd dword [eax+1], ecx
	xadd dword [eax+64], esi
	xadd qword [eax+1], rdi
	xadd qword [eax+64], rcx
	xadd [0x5f3], al
	xadd [0xa67], dx
	xadd [0x303], ebp
	xadd [0x660], rbx
