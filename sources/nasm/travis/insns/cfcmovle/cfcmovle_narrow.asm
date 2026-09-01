	cfcmovle cx, ax
	cfcmovle cx, si
	cfcmovle esi, esi
	cfcmovle edx, ebp
	cfcmovle dx, di
	cfcmovle bp, bp

%ifdef ERROR
	cfcmovle rax, rcx
	cfcmovle qword [0xf8f], rbx
	cfcmovle r15w, r12w
	cfcmovle r13d, r14d
	cfcmovle r15, r14
	cfcmovle r8w, r15w
	cfcmovle r20w, r31w
	cfcmovle r24d, r20d
	cfcmovle r23, r24
	cfcmovle r19w, r16w
	cfcmovle qword [eax+1], rcx
	cfcmovle qword [eax+64], rax
	cfcmovle rcx, qword [eax+1]
	cfcmovle rbx, qword [eax+64]
	cfcmovle rdi, rsi, qword [eax+1]
	cfcmovle rax, rsi, qword [eax+64]
	cfcmovle [0xc41], rbp
	cfcmovle rsi, [0xd64]
	cfcmovle rdi, rbp, [0x3b3]
%endif
