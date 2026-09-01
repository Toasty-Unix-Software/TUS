	cfcmovnc di, ax
	cfcmovnc ax, di
	cfcmovnc ecx, eax
	cfcmovnc ebp, edi
	cfcmovnc dx, di
	cfcmovnc bx, ax

%ifdef ERROR
	cfcmovnc rbx, rcx
	cfcmovnc qword [0xbe5], rdi
	cfcmovnc r12w, r15w
	cfcmovnc r9d, r15d
	cfcmovnc r11, r11
	cfcmovnc r15w, r9w
	cfcmovnc r25w, r17w
	cfcmovnc r18d, r16d
	cfcmovnc r30, r26
	cfcmovnc r18w, r31w
	cfcmovnc qword [eax+1], rbx
	cfcmovnc qword [eax+64], rdi
	cfcmovnc rax, qword [eax+1]
	cfcmovnc rax, qword [eax+64]
	cfcmovnc rsi, rax, qword [eax+1]
	cfcmovnc rdx, rdx, qword [eax+64]
	cfcmovnc [0x183], rdx
	cfcmovnc rbx, [0x6da]
	cfcmovnc rbx, rbx, [0xb63]
%endif
