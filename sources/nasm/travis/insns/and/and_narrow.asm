	and cl, dl
	and al, bl
	and word [0x35a], bp
	and bx, dx
	and edx, esi
	and edi, edx

%ifdef ERROR
	and rax, rax
	and rbx, rcx
	and r15b, r10b
	and r13w, r15w
	and r14d, r9d
	and r9, r13
	and r23b, r18b
	and r16w, r25w
	and r18d, r19d
	and r27, r16
	and qword [eax+1], rcx
	and qword [eax+64], rax
	and rsi, qword [eax+1]
	and rbx, qword [eax+64]
	and qword [eax+1], 44
	and qword [eax+64], -89
	and qword [eax+1], -275284148
	and qword [eax+64], 263899614
	and rsi, rbx, qword [eax+1]
	and rdi, rdx, qword [eax+64]
	and rax, qword [eax+1], rbp
	and rax, qword [eax+64], rbx
	and rax, qword [eax+1], -60
	and rax, qword [eax+64], -62
	and rcx, qword [eax+1], -506441116
	and rbp, qword [eax+64], 188036657
	and [0x96b], rbx
	and rsi, [0xc67]
	and [0xc3a], 17
	and [0x801], -308360386
	and rsi, rcx, [0x705]
	and rcx, [0x237], rbp
	and rsi, [0x255], -7
	and rcx, [0x22a], 468030773
%endif
