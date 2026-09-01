	ccmpc 9, al, dl
	ccmpc 3, bl, cl
	ccmpc 8, word [0x5e9], cx
	ccmpc 11, ax, si
	ccmpc 13, dword [0x544], edi
	ccmpc 6, eax, ebx

%ifdef ERROR
	ccmpc 14, rax, rsi
	ccmpc 15, rdi, rsi
	ccmpc 2, r8b, r11b
	ccmpc 15, r8w, r15w
	ccmpc 8, r9d, r11d
	ccmpc 2, r15, r15
	ccmpc 9, r27b, r29b
	ccmpc 13, r16w, r29w
	ccmpc 8, r30d, r19d
	ccmpc 9, r17, r20
	ccmpc 14, qword [eax+1], rdx
	ccmpc 5, qword [eax+64], rsi
	ccmpc 10, rcx, qword [eax+1]
	ccmpc 7, rdi, qword [eax+64]
	ccmpc 7, qword [eax+1], 47
	ccmpc 8, qword [eax+64], 33
	ccmpc 8, qword [eax+1], 53357401
	ccmpc 11, qword [eax+64], -142029383
	ccmpc 14, [0xe32], rdi
	ccmpc 10, rax, [0x273]
	ccmpc 8, [0xaf0], -115
	ccmpc 3, [0xbac], 104916897
%endif
