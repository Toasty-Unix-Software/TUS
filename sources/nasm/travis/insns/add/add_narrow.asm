	add bl, bl
	add dl, bl
	add dx, si
	add di, di
	add dword [0xa9d], ecx
	add esi, ebp

%ifdef ERROR
	add rax, rcx
	add rbp, rdi
	add r14b, r13b
	add r14w, r12w
	add r12d, r15d
	add r12, r15
	add r20b, r24b
	add r19w, r16w
	add r22d, r27d
	add r25, r21
	add qword [eax+1], rbp
	add qword [eax+64], rbx
	add rcx, qword [eax+1]
	add rcx, qword [eax+64]
	add qword [eax+1], -7
	add qword [eax+64], -32
	add qword [eax+1], -196667217
	add qword [eax+64], -49947754
	add rsi, rdi, qword [eax+1]
	add rsi, rcx, qword [eax+64]
	add rdi, qword [eax+1], rsi
	add rax, qword [eax+64], rax
	add rbp, qword [eax+1], 68
	add rcx, qword [eax+64], 53
	add rdi, qword [eax+1], 455876284
	add rsi, qword [eax+64], -93805067
	add [0x9c6], rax
	add rax, [0x46d]
	add [0x23e], 26
	add [0xdf9], -30949254
	add rbp, rbx, [0xe78]
	add rdi, [0xf51], rsi
	add rsi, [0x8fe], -1
	add rdx, [0x5d5], 232948762
%endif
