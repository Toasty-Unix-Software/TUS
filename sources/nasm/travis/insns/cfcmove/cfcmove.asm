default rel
	cfcmove ax, ax
	cfcmove word [0xb24], ax
	cfcmove dword [0xee6], eax
	cfcmove eax, edx
	cfcmove rcx, rbp
	cfcmove rdi, rbp
	cfcmove di, dx
	cfcmove bp, word [0x31c]
	cfcmove r9w, r12w
	cfcmove r11d, r11d
	cfcmove r13, r10
	cfcmove r12w, r15w
	cfcmove r19w, r22w
	cfcmove r25d, r18d
	cfcmove r16, r31
	cfcmove r20w, r31w
	cfcmove word [eax+1], dx
	cfcmove word [eax+64], di
	cfcmove dword [eax+1], edx
	cfcmove dword [eax+64], edi
	cfcmove qword [eax+1], rcx
	cfcmove qword [eax+64], rax
	cfcmove bx, word [eax+1]
	cfcmove ax, word [eax+64]
	cfcmove edi, dword [eax+1]
	cfcmove edx, dword [eax+64]
	cfcmove rdx, qword [eax+1]
	cfcmove rcx, qword [eax+64]
	cfcmove ax, di, word [eax+1]
	cfcmove bp, si, word [eax+64]
	cfcmove edi, esi, dword [eax+1]
	cfcmove esi, esi, dword [eax+64]
	cfcmove rbp, rdx, qword [eax+1]
	cfcmove rsi, rax, qword [eax+64]
	cfcmove [0xd35], bp
	cfcmove [0x61a], ecx
	cfcmove [0x55b], rax
	cfcmove cx, [0xb69]
	cfcmove ebx, [0x291]
	cfcmove rax, [0xcc8]
	cfcmove bp, ax, [0x6c3]
	cfcmove esi, edx, [0x7f5]
	cfcmove rax, rax, [0x14e]
