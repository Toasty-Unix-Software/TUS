	cmpcxadd dword [0x9bc], edi, esi
	cmpcxadd dword [0x771], edi, edx

%ifdef ERROR
	cmpcxadd qword [0xb4e], rbp, rbx
	cmpcxadd qword [0xe72], rbx, rbx
	cmpcxadd dword [0xbd1], r14d, r8d
	cmpcxadd qword [0x62f], r8, r9
	cmpcxadd dword [0x60c], r31d, r20d
	cmpcxadd qword [0x396], r30, r20
	cmpcxadd qword [eax+1], rbx, rdx
	cmpcxadd qword [eax+64], rbp, rax
	cmpcxadd [0x36c], rbp, rdi
%endif
