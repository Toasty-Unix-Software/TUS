	rcr byte [0x905], 1
	rcr byte [0xcbc], 1
	rcr di, 1
	rcr bp, 1
	rcr dword [0xbb8], 1
	rcr eax, 1
	rcr rbp, 1
	rcr rax, 1

%ifdef ERROR
	rcr r12b, 1
	rcr r15w, 1
	rcr r10d, 1
	rcr r13, 1
	rcr r18b, 1
	rcr r28w, 1
	rcr r16d, 1
	rcr r30, 1
	rcr rsi, qword [eax+1], 1
	rcr rsi, qword [eax+64], 1
	rcr rbp, qword [eax+1], cl
	rcr rdi, qword [eax+64], cl
	rcr rdx, qword [eax+1], cx
	rcr rax, qword [eax+64], cx
	rcr rbx, qword [eax+1], ecx
	rcr rdi, qword [eax+64], ecx
	rcr rdx, qword [eax+1], 0x69
	rcr rsi, qword [eax+64], 0xb7
	rcr rcx, [0x648], 1
	rcr rax, [0xafc], cl
	rcr rax, [0x5cf], cx
	rcr rbp, [0xa24], ecx
	rcr rax, [0xbae], 0x9c
%endif
