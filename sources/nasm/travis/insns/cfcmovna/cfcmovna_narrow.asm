	cfcmovna di, si
	cfcmovna bx, ax
	cfcmovna edx, esi
	cfcmovna dword [0xdc9], ecx
	cfcmovna bx, cx
	cfcmovna si, di

%ifdef ERROR
	cfcmovna rcx, rbp
	cfcmovna rdi, rdi
	cfcmovna r15w, r9w
	cfcmovna r12d, r11d
	cfcmovna r9, r15
	cfcmovna r13w, r10w
	cfcmovna r20w, r28w
	cfcmovna r16d, r17d
	cfcmovna r23, r20
	cfcmovna r25w, r21w
	cfcmovna qword [eax+1], rdi
	cfcmovna qword [eax+64], rax
	cfcmovna rsi, qword [eax+1]
	cfcmovna rbp, qword [eax+64]
	cfcmovna rbx, rdx, qword [eax+1]
	cfcmovna rdi, rbp, qword [eax+64]
	cfcmovna [0x25b], rdx
	cfcmovna rdi, [0x449]
	cfcmovna rdi, rdi, [0x67b]
%endif
