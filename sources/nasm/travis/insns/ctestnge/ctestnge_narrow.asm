	ctestnge 8, dl, al
	ctestnge 7, al, al
	ctestnge 3, ax, bp
	ctestnge 9, dx, bp
	ctestnge 7, edi, edi
	ctestnge 3, dword [0x331], ebp

%ifdef ERROR
	ctestnge 6, rdi, rsi
	ctestnge 1, rsi, rdi
	ctestnge 5, r9b, r12b
	ctestnge 2, r9w, r9w
	ctestnge 7, r10d, r13d
	ctestnge 6, r12, r11
	ctestnge 7, r18b, r28b
	ctestnge 1, r27w, r27w
	ctestnge 10, r30d, r17d
	ctestnge 5, r31, r28
	ctestnge 13, qword [eax+1], rcx
	ctestnge 7, qword [eax+64], rdx
	ctestnge 8, qword [eax+1], 4978590
	ctestnge 2, qword [eax+64], -180127896
	ctestnge 1, [0x269], rbx
	ctestnge 8, [0x374], -475240841
%endif
