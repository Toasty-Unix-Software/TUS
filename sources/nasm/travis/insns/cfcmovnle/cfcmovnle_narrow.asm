	cfcmovnle word [0x568], di
	cfcmovnle word [0x56f], bp
	cfcmovnle eax, esi
	cfcmovnle ebx, esi
	cfcmovnle bp, si
	cfcmovnle cx, si

%ifdef ERROR
	cfcmovnle rcx, rdx
	cfcmovnle rcx, rdi
	cfcmovnle r9w, r15w
	cfcmovnle r8d, r8d
	cfcmovnle r9, r11
	cfcmovnle r8w, r14w
	cfcmovnle r18w, r23w
	cfcmovnle r22d, r23d
	cfcmovnle r31, r30
	cfcmovnle r26w, r27w
	cfcmovnle qword [eax+1], rcx
	cfcmovnle qword [eax+64], rsi
	cfcmovnle rdx, qword [eax+1]
	cfcmovnle rbp, qword [eax+64]
	cfcmovnle rbp, rbp, qword [eax+1]
	cfcmovnle rcx, rsi, qword [eax+64]
	cfcmovnle [0x289], rsi
	cfcmovnle rbx, [0xa81]
	cfcmovnle rax, rax, [0xfcb]
%endif
