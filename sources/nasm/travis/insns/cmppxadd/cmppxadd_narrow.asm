	cmppxadd dword [0x5ee], edx, ebp
	cmppxadd dword [0x6ee], edx, eax

%ifdef ERROR
	cmppxadd qword [0x82e], rsi, rsi
	cmppxadd qword [0x8e5], rdi, rsi
	cmppxadd dword [0xea6], r12d, r15d
	cmppxadd qword [0x4fd], r14, r13
	cmppxadd dword [0xe6c], r19d, r17d
	cmppxadd qword [0x78b], r23, r20
	cmppxadd qword [eax+1], rax, rdx
	cmppxadd qword [eax+64], rdi, rbp
	cmppxadd [0x188], rbp, rcx
%endif
