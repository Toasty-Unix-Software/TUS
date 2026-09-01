	cfcmove ax, ax
	cfcmove word [0xb24], ax
	cfcmove dword [0xee6], eax
	cfcmove eax, edx
	cfcmove di, dx
	cfcmove bp, word [0x31c]

%ifdef ERROR
	cfcmove rcx, rbp
	cfcmove rdi, rbp
	cfcmove r9w, r12w
	cfcmove r11d, r11d
	cfcmove r13, r10
	cfcmove r12w, r15w
	cfcmove r19w, r22w
	cfcmove r25d, r18d
	cfcmove r16, r31
	cfcmove r20w, r31w
	cfcmove qword [eax+1], rcx
	cfcmove qword [eax+64], rax
	cfcmove rdx, qword [eax+1]
	cfcmove rcx, qword [eax+64]
	cfcmove rbp, rdx, qword [eax+1]
	cfcmove rsi, rax, qword [eax+64]
	cfcmove [0x55b], rax
	cfcmove rax, [0xcc8]
	cfcmove rax, rax, [0x14e]
%endif
