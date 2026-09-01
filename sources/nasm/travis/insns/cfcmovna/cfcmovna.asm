default rel
	cfcmovna di, si
	cfcmovna bx, ax
	cfcmovna edx, esi
	cfcmovna dword [0xdc9], ecx
	cfcmovna rcx, rbp
	cfcmovna rdi, rdi
	cfcmovna bx, cx
	cfcmovna si, di
	cfcmovna r15w, r9w
	cfcmovna r12d, r11d
	cfcmovna r9, r15
	cfcmovna r13w, r10w
	cfcmovna r20w, r28w
	cfcmovna r16d, r17d
	cfcmovna r23, r20
	cfcmovna r25w, r21w
	cfcmovna word [eax+1], cx
	cfcmovna word [eax+64], dx
	cfcmovna dword [eax+1], edx
	cfcmovna dword [eax+64], ebx
	cfcmovna qword [eax+1], rdi
	cfcmovna qword [eax+64], rax
	cfcmovna di, word [eax+1]
	cfcmovna bp, word [eax+64]
	cfcmovna ecx, dword [eax+1]
	cfcmovna ebp, dword [eax+64]
	cfcmovna rsi, qword [eax+1]
	cfcmovna rbp, qword [eax+64]
	cfcmovna bp, si, word [eax+1]
	cfcmovna di, bp, word [eax+64]
	cfcmovna edx, edx, dword [eax+1]
	cfcmovna ecx, ebp, dword [eax+64]
	cfcmovna rbx, rdx, qword [eax+1]
	cfcmovna rdi, rbp, qword [eax+64]
	cfcmovna [0x59b], si
	cfcmovna [0xd71], eax
	cfcmovna [0x25b], rdx
	cfcmovna bx, [0x5ea]
	cfcmovna edx, [0x46b]
	cfcmovna rdi, [0x449]
	cfcmovna cx, dx, [0xf08]
	cfcmovna ecx, ebx, [0x605]
	cfcmovna rdi, rdi, [0x67b]
