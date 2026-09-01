	blsmsk eax, edx
	blsmsk edi, dword [0xaaa]

%ifdef ERROR
	blsmsk rdi, rcx
	blsmsk rdx, rdi
	blsmsk r9d, r9d
	blsmsk r14, r9
	blsmsk r27d, r19d
	blsmsk r24, r18
	blsmsk rbp, qword [eax+1]
	blsmsk rcx, qword [eax+64]
	blsmsk rax, [0xb06]
%endif
