	blsic ecx, ebp
	blsic ebx, ecx

%ifdef ERROR
	blsic rcx, rbx
	blsic rsi, rcx
	blsic r13d, r9d
	blsic r14, r9
	blsic rdx, qword [eax+1]
	blsic rcx, qword [eax+64]
	blsic rsi, [0x45a]
%endif
