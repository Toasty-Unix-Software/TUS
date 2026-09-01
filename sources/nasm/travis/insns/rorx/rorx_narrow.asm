	rorx edi, dword [0xb58], 0x74
	rorx ebx, ebp, 0xfa

%ifdef ERROR
	rorx rbp, qword [0x1f0], 0xa9
	rorx rcx, rbx, 0x1c
	rorx r9d, r9d, 0x51
	rorx r9, r12, 0xfc
	rorx r18d, r31d, 0xb
	rorx r17, r20, 0x69
	rorx rdi, qword [eax+1], 0x9d
	rorx rdi, qword [eax+64], 0x39
	rorx rdi, [0x545], 0xd3
%endif
