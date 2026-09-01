	ccmpa 10, dl, bl
	ccmpa 11, dl, dl
	ccmpa 0, ax, di
	ccmpa 4, bx, ax
	ccmpa 13, dword [0x89e], ebx
	ccmpa 1, ecx, ebp

%ifdef ERROR
	ccmpa 14, rsi, rsi
	ccmpa 6, qword [0x3d6], rcx
	ccmpa 4, r13b, r14b
	ccmpa 0, r15w, r12w
	ccmpa 9, r12d, r13d
	ccmpa 2, r12, r13
	ccmpa 13, r28b, r25b
	ccmpa 3, r22w, r16w
	ccmpa 2, r27d, r25d
	ccmpa 9, r29, r24
	ccmpa 11, qword [eax+1], rbp
	ccmpa 9, qword [eax+64], rbx
	ccmpa 3, rbx, qword [eax+1]
	ccmpa 8, rdx, qword [eax+64]
	ccmpa 3, qword [eax+1], 35
	ccmpa 12, qword [eax+64], -73
	ccmpa 0, qword [eax+1], 477395961
	ccmpa 4, qword [eax+64], 326080965
	ccmpa 0, [0x19c], rdx
	ccmpa 0, rbp, [0xc25]
	ccmpa 6, [0xd9b], 21
	ccmpa 3, [0xf98], -277723076
%endif
