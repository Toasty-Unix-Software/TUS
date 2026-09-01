	cfcmovpo word [0xafe], cx
	cfcmovpo cx, ax
	cfcmovpo ecx, ebx
	cfcmovpo dword [0x5ce], ebp
	cfcmovpo bp, dx
	cfcmovpo bp, si

%ifdef ERROR
	cfcmovpo rax, rbp
	cfcmovpo rdi, rax
	cfcmovpo r11w, r15w
	cfcmovpo r11d, r9d
	cfcmovpo r14, r15
	cfcmovpo r12w, r9w
	cfcmovpo r27w, r20w
	cfcmovpo r18d, r17d
	cfcmovpo r31, r25
	cfcmovpo r22w, r23w
	cfcmovpo qword [eax+1], rax
	cfcmovpo qword [eax+64], rcx
	cfcmovpo rbp, qword [eax+1]
	cfcmovpo rbx, qword [eax+64]
	cfcmovpo rax, rcx, qword [eax+1]
	cfcmovpo rdx, rbp, qword [eax+64]
	cfcmovpo [0x584], rsi
	cfcmovpo rsi, [0x81e]
	cfcmovpo rbp, rdi, [0x356]
%endif
