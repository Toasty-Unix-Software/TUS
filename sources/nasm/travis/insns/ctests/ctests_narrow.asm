	ctests 12, byte [0x30d], al
	ctests 11, bl, dl
	ctests 0, dx, ax
	ctests 9, bx, bp
	ctests 4, dword [0x626], edx
	ctests 11, dword [0xdd1], edi

%ifdef ERROR
	ctests 14, rcx, rcx
	ctests 11, qword [0x25f], rcx
	ctests 5, r13b, r14b
	ctests 6, r13w, r8w
	ctests 7, r10d, r11d
	ctests 5, r12, r9
	ctests 11, r19b, r27b
	ctests 3, r29w, r27w
	ctests 5, r25d, r16d
	ctests 14, r24, r21
	ctests 12, qword [eax+1], rax
	ctests 13, qword [eax+64], rcx
	ctests 0, qword [eax+1], 505883947
	ctests 10, qword [eax+64], 178059430
	ctests 8, [0xcfe], rbx
	ctests 13, [0x2ae], -85772268
%endif
