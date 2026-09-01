	cfcmovpe bx, cx
	cfcmovpe bp, ax
	cfcmovpe ecx, ebp
	cfcmovpe eax, ebp
	cfcmovpe bp, word [0xd05]
	cfcmovpe cx, ax

%ifdef ERROR
	cfcmovpe rdx, rbx
	cfcmovpe rdx, rdi
	cfcmovpe r11w, r12w
	cfcmovpe r10d, r15d
	cfcmovpe r11, r12
	cfcmovpe r14w, r8w
	cfcmovpe r23w, r20w
	cfcmovpe r20d, r29d
	cfcmovpe r26, r20
	cfcmovpe r29w, r18w
	cfcmovpe qword [eax+1], rdx
	cfcmovpe qword [eax+64], rbx
	cfcmovpe rcx, qword [eax+1]
	cfcmovpe rax, qword [eax+64]
	cfcmovpe rsi, rdi, qword [eax+1]
	cfcmovpe rbx, rsi, qword [eax+64]
	cfcmovpe [0x8f2], rdi
	cfcmovpe rax, [0x59f]
	cfcmovpe rax, rbx, [0xb9e]
%endif
