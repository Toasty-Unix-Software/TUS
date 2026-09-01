	ccmpna 7, byte [0xfa6], bl
	ccmpna 6, byte [0x551], al
	ccmpna 0, word [0xa7a], bx
	ccmpna 13, bp, si
	ccmpna 12, eax, esi
	ccmpna 11, edi, ebp

%ifdef ERROR
	ccmpna 10, rbp, rbp
	ccmpna 1, rbx, rcx
	ccmpna 13, r12b, r12b
	ccmpna 7, r9w, r9w
	ccmpna 6, r10d, r15d
	ccmpna 9, r9, r11
	ccmpna 5, r31b, r19b
	ccmpna 7, r29w, r22w
	ccmpna 1, r29d, r31d
	ccmpna 7, r24, r17
	ccmpna 6, qword [eax+1], rbx
	ccmpna 12, qword [eax+64], rcx
	ccmpna 9, rbp, qword [eax+1]
	ccmpna 8, rcx, qword [eax+64]
	ccmpna 11, qword [eax+1], -36
	ccmpna 2, qword [eax+64], 119
	ccmpna 0, qword [eax+1], 465072673
	ccmpna 2, qword [eax+64], 121930368
	ccmpna 9, [0x95f], rdx
	ccmpna 8, rbx, [0x1e3]
	ccmpna 8, [0x269], -103
	ccmpna 7, [0xb8f], -356894819
%endif
