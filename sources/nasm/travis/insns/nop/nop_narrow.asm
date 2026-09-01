	nop
	nop
	nop bp
	nop cx
	nop ebx
	nop dword [0xe32]
	nop qword [0xa8d]
	nop qword [0xfc1]

%ifdef ERROR
	nop r10w
	nop r13d
	nop r9
	nop r16w
	nop r22d
	nop r24
	nop 0x5f, rsi, qword [eax+1]
	nop 0x1d, rdx, qword [eax+64]
	nop 0x22, rbp, [0x1ee]
%endif
