	ctestnl 11, bl, dl
	ctestnl 7, dl, al
	ctestnl 5, ax, di
	ctestnl 9, di, dx
	ctestnl 15, eax, ecx
	ctestnl 7, edi, ecx

%ifdef ERROR
	ctestnl 10, rbp, rcx
	ctestnl 2, qword [0x311], rax
	ctestnl 4, r8b, r13b
	ctestnl 0, r14w, r12w
	ctestnl 8, r15d, r14d
	ctestnl 13, r13, r15
	ctestnl 10, r29b, r24b
	ctestnl 13, r28w, r19w
	ctestnl 1, r17d, r18d
	ctestnl 6, r25, r29
	ctestnl 15, qword [eax+1], rbx
	ctestnl 1, qword [eax+64], rdi
	ctestnl 2, qword [eax+1], 412969038
	ctestnl 9, qword [eax+64], -59803463
	ctestnl 15, [0xfe9], rbx
	ctestnl 10, [0x98d], 471071136
%endif
