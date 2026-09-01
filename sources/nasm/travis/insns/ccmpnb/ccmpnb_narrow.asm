	ccmpnb 2, byte [0x683], cl
	ccmpnb 2, cl, cl
	ccmpnb 7, word [0x8c2], cx
	ccmpnb 2, dx, bp
	ccmpnb 8, ecx, ebp
	ccmpnb 5, dword [0xf3c], ebx

%ifdef ERROR
	ccmpnb 3, rdi, rax
	ccmpnb 15, rbx, rbx
	ccmpnb 2, r9b, r11b
	ccmpnb 8, r10w, r8w
	ccmpnb 9, r10d, r12d
	ccmpnb 11, r13, r9
	ccmpnb 11, r20b, r23b
	ccmpnb 10, r20w, r29w
	ccmpnb 8, r28d, r21d
	ccmpnb 7, r18, r21
	ccmpnb 13, qword [eax+1], rax
	ccmpnb 12, qword [eax+64], rsi
	ccmpnb 0, rcx, qword [eax+1]
	ccmpnb 1, rcx, qword [eax+64]
	ccmpnb 3, qword [eax+1], 53
	ccmpnb 11, qword [eax+64], -79
	ccmpnb 3, qword [eax+1], -314858551
	ccmpnb 4, qword [eax+64], 244465706
	ccmpnb 1, [0x7c1], rdi
	ccmpnb 14, rbx, [0x6b8]
	ccmpnb 1, [0xf24], 85
	ccmpnb 5, [0xfb6], -424977228
%endif
