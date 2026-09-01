	cfcmovb word [0xe97], cx
	cfcmovb word [0xdfb], dx
	cfcmovb esi, ebp
	cfcmovb edi, eax
	cfcmovb bx, bp
	cfcmovb si, word [0x1da]

%ifdef ERROR
	cfcmovb rcx, rdi
	cfcmovb rbx, rbp
	cfcmovb r10w, r15w
	cfcmovb r9d, r8d
	cfcmovb r11, r9
	cfcmovb r13w, r11w
	cfcmovb r21w, r28w
	cfcmovb r20d, r18d
	cfcmovb r17, r21
	cfcmovb r26w, r31w
	cfcmovb qword [eax+1], rcx
	cfcmovb qword [eax+64], rax
	cfcmovb rcx, qword [eax+1]
	cfcmovb rbx, qword [eax+64]
	cfcmovb rsi, rdi, qword [eax+1]
	cfcmovb rax, rcx, qword [eax+64]
	cfcmovb [0x16e], rbx
	cfcmovb rcx, [0x94f]
	cfcmovb rbp, rdi, [0x2da]
%endif
