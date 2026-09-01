default rel
	blsic ecx, ebp
	blsic ebx, ecx
	blsic rcx, rbx
	blsic rsi, rcx
	blsic r13d, r9d
	blsic r14, r9
	blsic edi, dword [eax+1]
	blsic edi, dword [eax+64]
	blsic rdx, qword [eax+1]
	blsic rcx, qword [eax+64]
	blsic ecx, [0xdbe]
	blsic rsi, [0x45a]
