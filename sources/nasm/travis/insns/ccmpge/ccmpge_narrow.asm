	ccmpge 7, bl, bl
	ccmpge 4, bl, al
	ccmpge 13, ax, dx
	ccmpge 10, bx, dx
	ccmpge 11, edx, edx
	ccmpge 1, dword [0xdde], edi

%ifdef ERROR
	ccmpge 4, rbx, rbx
	ccmpge 15, qword [0xb98], rdx
	ccmpge 12, r11b, r12b
	ccmpge 11, r12w, r8w
	ccmpge 6, r8d, r12d
	ccmpge 1, r13, r13
	ccmpge 9, r20b, r21b
	ccmpge 9, r18w, r18w
	ccmpge 5, r17d, r18d
	ccmpge 7, r26, r18
	ccmpge 0, qword [eax+1], rax
	ccmpge 11, qword [eax+64], rbx
	ccmpge 11, rdi, qword [eax+1]
	ccmpge 1, rdx, qword [eax+64]
	ccmpge 14, qword [eax+1], 100
	ccmpge 12, qword [eax+64], 46
	ccmpge 1, qword [eax+1], 303069272
	ccmpge 7, qword [eax+64], 184283815
	ccmpge 11, [0x692], rax
	ccmpge 11, rcx, [0xbd9]
	ccmpge 9, [0x178], 38
	ccmpge 15, [0x7cf], 86009347
%endif
