	sub al, al
	sub byte [0x1f3], cl
	sub word [0x24b], bx
	sub dx, bp
	sub edx, ebp
	sub ebx, ecx

%ifdef ERROR
	sub rbx, rcx
	sub rbx, rsi
	sub r14b, r10b
	sub r14w, r11w
	sub r15d, r15d
	sub r13, r13
	sub r26b, r27b
	sub r16w, r24w
	sub r30d, r31d
	sub r24, r18
	sub qword [eax+1], rbp
	sub qword [eax+64], rdx
	sub rdx, qword [eax+1]
	sub rcx, qword [eax+64]
	sub qword [eax+1], -15
	sub qword [eax+64], -91
	sub qword [eax+1], -391842762
	sub qword [eax+64], -227951707
	sub rbp, rsi, qword [eax+1]
	sub rbp, rdx, qword [eax+64]
	sub rbp, qword [eax+1], rdi
	sub rbp, qword [eax+64], rbp
	sub rbx, qword [eax+1], -12
	sub rdi, qword [eax+64], -55
	sub rax, qword [eax+1], -507148648
	sub rcx, qword [eax+64], -395403989
	sub [0xd9e], rbp
	sub rbp, [0x9e3]
	sub [0xb18], 54
	sub [0xcc0], -276330528
	sub rdi, rbx, [0x5b0]
	sub rbp, [0x4bb], rdi
	sub rcx, [0x95b], -97
	sub rbp, [0xc4c], -302261169
%endif
