	ccmpt 12, bl, cl
	ccmpt 5, dl, dl
	ccmpt 1, word [0xac6], bp
	ccmpt 0, di, si
	ccmpt 0, dword [0x537], esi
	ccmpt 3, ebx, edx

%ifdef ERROR
	ccmpt 0, rdi, rax
	ccmpt 3, rbp, rdi
	ccmpt 6, r8b, r15b
	ccmpt 9, r12w, r15w
	ccmpt 4, r10d, r9d
	ccmpt 8, r8, r10
	ccmpt 0, r27b, r18b
	ccmpt 11, r18w, r20w
	ccmpt 3, r27d, r21d
	ccmpt 6, r28, r29
	ccmpt 0, qword [eax+1], rax
	ccmpt 12, qword [eax+64], rcx
	ccmpt 3, rbp, qword [eax+1]
	ccmpt 4, rdi, qword [eax+64]
	ccmpt 9, qword [eax+1], -125
	ccmpt 15, qword [eax+64], 79
	ccmpt 0, qword [eax+1], 274066865
	ccmpt 12, qword [eax+64], -20522462
	ccmpt 10, [0x20a], rcx
	ccmpt 0, rdi, [0xab1]
	ccmpt 3, [0xfc9], -65
	ccmpt 10, [0xef9], 524067526
%endif
