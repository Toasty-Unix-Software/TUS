	ccmpbe 6, dl, bl
	ccmpbe 5, byte [0x3b3], bl
	ccmpbe 13, word [0xb57], bp
	ccmpbe 0, word [0xdd8], di
	ccmpbe 10, esi, edi
	ccmpbe 0, ebx, ecx

%ifdef ERROR
	ccmpbe 12, rdx, rbx
	ccmpbe 12, rdx, rdi
	ccmpbe 9, r8b, r12b
	ccmpbe 2, r15w, r9w
	ccmpbe 11, r15d, r12d
	ccmpbe 12, r13, r8
	ccmpbe 3, r28b, r19b
	ccmpbe 6, r18w, r27w
	ccmpbe 4, r22d, r23d
	ccmpbe 2, r17, r23
	ccmpbe 1, qword [eax+1], rdx
	ccmpbe 12, qword [eax+64], rbx
	ccmpbe 13, rax, qword [eax+1]
	ccmpbe 10, rdx, qword [eax+64]
	ccmpbe 2, qword [eax+1], 12
	ccmpbe 8, qword [eax+64], 43
	ccmpbe 9, qword [eax+1], -207183652
	ccmpbe 12, qword [eax+64], -332313569
	ccmpbe 5, [0x11a], rcx
	ccmpbe 11, rsi, [0x509]
	ccmpbe 14, [0x557], 122
	ccmpbe 9, [0xebc], -470759931
%endif
