	t1mskc eax, eax
	t1mskc esi, edi

%ifdef ERROR
	t1mskc rbp, rax
	t1mskc rbp, qword [0xaef]
	t1mskc r13d, r10d
	t1mskc r14, r14
	t1mskc rdx, qword [eax+1]
	t1mskc rdx, qword [eax+64]
	t1mskc rcx, [0x81f]
%endif
