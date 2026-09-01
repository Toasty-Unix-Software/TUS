	cfcmovge bp, cx
	cfcmovge cx, bx
	cfcmovge dword [0xe2e], eax
	cfcmovge dword [0x990], ebx
	cfcmovge cx, di
	cfcmovge cx, word [0xc3b]

%ifdef ERROR
	cfcmovge rdx, rbp
	cfcmovge rdx, rax
	cfcmovge r14w, r13w
	cfcmovge r13d, r8d
	cfcmovge r15, r11
	cfcmovge r10w, r13w
	cfcmovge r29w, r26w
	cfcmovge r24d, r30d
	cfcmovge r19, r16
	cfcmovge r31w, r18w
	cfcmovge qword [eax+1], rsi
	cfcmovge qword [eax+64], rsi
	cfcmovge rdi, qword [eax+1]
	cfcmovge rax, qword [eax+64]
	cfcmovge rax, rbp, qword [eax+1]
	cfcmovge rbp, rcx, qword [eax+64]
	cfcmovge [0xb56], rsi
	cfcmovge rdi, [0x41b]
	cfcmovge rdx, rbp, [0x872]
%endif
