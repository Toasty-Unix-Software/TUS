default rel
	cfcmovb word [0xe97], cx
	cfcmovb word [0xdfb], dx
	cfcmovb esi, ebp
	cfcmovb edi, eax
	cfcmovb rcx, rdi
	cfcmovb rbx, rbp
	cfcmovb bx, bp
	cfcmovb si, word [0x1da]
	cfcmovb r10w, r15w
	cfcmovb r9d, r8d
	cfcmovb r11, r9
	cfcmovb r13w, r11w
	cfcmovb r21w, r28w
	cfcmovb r20d, r18d
	cfcmovb r17, r21
	cfcmovb r26w, r31w
	cfcmovb word [eax+1], dx
	cfcmovb word [eax+64], cx
	cfcmovb dword [eax+1], ecx
	cfcmovb dword [eax+64], ebp
	cfcmovb qword [eax+1], rcx
	cfcmovb qword [eax+64], rax
	cfcmovb dx, word [eax+1]
	cfcmovb di, word [eax+64]
	cfcmovb ebp, dword [eax+1]
	cfcmovb edx, dword [eax+64]
	cfcmovb rcx, qword [eax+1]
	cfcmovb rbx, qword [eax+64]
	cfcmovb bp, di, word [eax+1]
	cfcmovb si, bp, word [eax+64]
	cfcmovb edi, ecx, dword [eax+1]
	cfcmovb eax, eax, dword [eax+64]
	cfcmovb rsi, rdi, qword [eax+1]
	cfcmovb rax, rcx, qword [eax+64]
	cfcmovb [0x98e], bx
	cfcmovb [0x874], edi
	cfcmovb [0x16e], rbx
	cfcmovb dx, [0xc65]
	cfcmovb ebx, [0xda4]
	cfcmovb rcx, [0x94f]
	cfcmovb bp, si, [0x784]
	cfcmovb ecx, ebp, [0x810]
	cfcmovb rbp, rdi, [0x2da]
