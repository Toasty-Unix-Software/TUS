	cfcmovo ax, bx
	cfcmovo word [0xf69], si
	cfcmovo ebp, ebx
	cfcmovo edi, ecx
	cfcmovo cx, bx
	cfcmovo bp, dx

%ifdef ERROR
	cfcmovo rcx, rdx
	cfcmovo qword [0x5c3], rdi
	cfcmovo r14w, r14w
	cfcmovo r11d, r8d
	cfcmovo r8, r12
	cfcmovo r9w, r8w
	cfcmovo r27w, r25w
	cfcmovo r19d, r27d
	cfcmovo r21, r29
	cfcmovo r26w, r21w
	cfcmovo qword [eax+1], rcx
	cfcmovo qword [eax+64], rbp
	cfcmovo rcx, qword [eax+1]
	cfcmovo rdx, qword [eax+64]
	cfcmovo rdx, rsi, qword [eax+1]
	cfcmovo rdx, rbx, qword [eax+64]
	cfcmovo [0xe3d], rbp
	cfcmovo rsi, [0x283]
	cfcmovo rbx, rsi, [0x968]
%endif
