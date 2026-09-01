	cmppexadd dword [0xc02], edi, ecx
	cmppexadd dword [0x480], edi, ecx

%ifdef ERROR
	cmppexadd qword [0xa9b], rsi, rax
	cmppexadd qword [0x830], rax, rdx
	cmppexadd dword [0x2dc], r10d, r13d
	cmppexadd qword [0x3a9], r12, r8
	cmppexadd dword [0xce9], r27d, r28d
	cmppexadd qword [0x530], r24, r30
	cmppexadd qword [eax+1], rbx, rcx
	cmppexadd qword [eax+64], rax, rsi
	cmppexadd [0x962], rbx, rcx
%endif
