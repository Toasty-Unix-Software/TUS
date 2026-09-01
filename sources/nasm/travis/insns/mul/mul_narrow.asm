	mul byte [0x148]
	mul cl
	mul word [0xe70]
	mul word [0x38a]
	mul ebp
	mul ebp
	mul rdx
	mul rdi

%ifdef ERROR
	mul r11b
	mul r11w
	mul r10d
	mul r10
	mul r17b
	mul r25w
	mul r28d
	mul r17
	mul rax, rbx, qword [eax+1]
	mul rbp, rcx, qword [eax+64]
	mul rcx, qword [eax+1]
	mul rax, qword [eax+64]
	mul rsi, qword [eax+1], 1
	mul rsi, qword [eax+64], 121
	mul rbp, qword [eax+1], 413629314
	mul rsi, qword [eax+64], -220617720
	mul rax, rcx, qword [eax+1]
	mul rax, rsi, qword [eax+64]
	mul rsi, qword [eax+1], 96
	mul rdx, qword [eax+64], 47
	mul rcx, qword [eax+1], -389714665
	mul rbx, qword [eax+64], -242256023
	mul rsi, rbp, [0x3bf]
	mul rcx, [0x1b2]
	mul rbx, [0xee2], 86
	mul rbx, [0x7a2], -71746408
	mul rax, rdx, [0xf4f]
	mul rdi, [0xdc8], -13
	mul rbp, [0x5a3], -518170626
%endif
