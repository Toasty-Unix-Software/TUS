	blci edx, ebx
	blci edx, ebx

%ifdef ERROR
	blci rdx, rdi
	blci rbx, rsi
	blci r10d, r14d
	blci r13, r10
	blci rbp, qword [eax+1]
	blci rdi, qword [eax+64]
	blci rax, [0xd36]
%endif
