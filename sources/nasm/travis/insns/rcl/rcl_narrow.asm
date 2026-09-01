	rcl dl, 1
	rcl byte [0xfbe], 1
	rcl cx, 1
	rcl ax, 1
	rcl eax, 1
	rcl dword [0xc4e], 1
	rcl rdx, 1
	rcl rsi, 1

%ifdef ERROR
	rcl r15b, 1
	rcl r9w, 1
	rcl r12d, 1
	rcl r10, 1
	rcl r24b, 1
	rcl r29w, 1
	rcl r26d, 1
	rcl r25, 1
	rcl rbp, qword [eax+1], 1
	rcl rbp, qword [eax+64], 1
	rcl rdx, qword [eax+1], cl
	rcl rbp, qword [eax+64], cl
	rcl rsi, qword [eax+1], cx
	rcl rsi, qword [eax+64], cx
	rcl rbp, qword [eax+1], ecx
	rcl rax, qword [eax+64], ecx
	rcl rbx, qword [eax+1], 0xda
	rcl rax, qword [eax+64], 0xf6
	rcl rdx, [0x456], 1
	rcl rdi, [0xbf9], cl
	rcl rax, [0x5cf], cx
	rcl rdx, [0x17f], ecx
	rcl rdx, [0xe9a], 0xbd
%endif
