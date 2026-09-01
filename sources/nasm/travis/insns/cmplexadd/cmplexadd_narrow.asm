	cmplexadd dword [0x497], ebx, ebx
	cmplexadd dword [0x34b], ecx, esi

%ifdef ERROR
	cmplexadd qword [0x4fe], rsi, rcx
	cmplexadd qword [0x59b], rbp, rsi
	cmplexadd dword [0x474], r15d, r13d
	cmplexadd qword [0xefe], r15, r10
	cmplexadd dword [0x1ab], r29d, r16d
	cmplexadd qword [0x426], r25, r28
	cmplexadd qword [eax+1], rdx, rbp
	cmplexadd qword [eax+64], rbp, rcx
	cmplexadd [0x6ee], rbx, rdi
%endif
