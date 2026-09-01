	pdep eax, edi, ebp
	pdep eax, ebp
	pdep esi, edx, dword [0x8c9]

%ifdef ERROR
	pdep rax, rcx, rcx
	pdep rax, rcx
	pdep rsi, rsi, rbp
	pdep r11d, r8d, r12d
	pdep r12, r14, r10
	pdep r26d, r27d, r27d
	pdep r28, r18, r28
	pdep rbp, rsi, qword [eax+1]
	pdep rbx, rsi, qword [eax+64]
	pdep rdx, rdi, [0x993]
%endif
