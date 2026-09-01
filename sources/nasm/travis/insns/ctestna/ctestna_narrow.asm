	ctestna 12, bl, bl
	ctestna 1, cl, bl
	ctestna 1, ax, dx
	ctestna 9, word [0x611], bp
	ctestna 12, ebx, ebp
	ctestna 9, dword [0x756], ecx

%ifdef ERROR
	ctestna 15, qword [0x6af], rcx
	ctestna 15, rdx, rbp
	ctestna 11, r10b, r8b
	ctestna 2, r10w, r12w
	ctestna 10, r13d, r13d
	ctestna 2, r12, r11
	ctestna 1, r20b, r17b
	ctestna 15, r18w, r29w
	ctestna 8, r24d, r18d
	ctestna 11, r17, r24
	ctestna 1, qword [eax+1], rbx
	ctestna 10, qword [eax+64], rax
	ctestna 0, qword [eax+1], 404935948
	ctestna 12, qword [eax+64], 486058033
	ctestna 4, [0x20e], rsi
	ctestna 4, [0xa6b], 505499774
%endif
