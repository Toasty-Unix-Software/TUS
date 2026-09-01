	bextr ebp, eax, eax
	bextr ebp, eax
	bextr edx, dword [0xc8d], eax
	bextr edi, dword [0x6a1], 0x162e66f4
	bextr edi, 0x162e66f4
	bextr edx, dword [0x9ba], 0x32e788d0

%ifdef ERROR
	bextr rcx, rbx, rbx
	bextr rcx, rbx
	bextr rsi, rbp, rsi
	bextr rbp, rax, 0x8e77e79
	bextr rbp, 0x8e77e79
	bextr rbx, qword [0xef0], 0x32faafb5
	bextr r13d, r14d, r15d
	bextr r10, r14, r9
	bextr r8d, r8d, 0x2ca3750d
	bextr r8, r15, 0x385a9676
	bextr rcx, qword [eax+1], rcx
	bextr rdi, qword [eax+64], rbp
	bextr rbx, qword [eax+1], 0x18b8a0b7
	bextr rcx, qword [eax+64], 0xc6b61d5
	bextr rbp, [0xdab], rsi
	bextr rax, [0xd9e], 0x3aa5b601
%endif
