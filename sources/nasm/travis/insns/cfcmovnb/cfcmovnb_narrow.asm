	cfcmovnb bx, ax
	cfcmovnb cx, dx
	cfcmovnb eax, eax
	cfcmovnb edx, ebp
	cfcmovnb dx, word [0x28b]
	cfcmovnb cx, di

%ifdef ERROR
	cfcmovnb rcx, rax
	cfcmovnb rdx, rcx
	cfcmovnb r13w, r9w
	cfcmovnb r12d, r9d
	cfcmovnb r13, r14
	cfcmovnb r12w, r9w
	cfcmovnb r31w, r16w
	cfcmovnb r16d, r22d
	cfcmovnb r31, r22
	cfcmovnb r24w, r20w
	cfcmovnb qword [eax+1], rcx
	cfcmovnb qword [eax+64], rsi
	cfcmovnb rbx, qword [eax+1]
	cfcmovnb rbx, qword [eax+64]
	cfcmovnb rcx, rcx, qword [eax+1]
	cfcmovnb rax, rdx, qword [eax+64]
	cfcmovnb [0xbb9], rdi
	cfcmovnb rdx, [0x65f]
	cfcmovnb rax, rbx, [0x756]
%endif
