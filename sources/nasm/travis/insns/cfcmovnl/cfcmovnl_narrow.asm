	cfcmovnl si, bp
	cfcmovnl word [0xc66], di
	cfcmovnl eax, esi
	cfcmovnl dword [0x410], edi
	cfcmovnl si, word [0x203]
	cfcmovnl bp, ax

%ifdef ERROR
	cfcmovnl rax, rbp
	cfcmovnl qword [0x58a], rbp
	cfcmovnl r12w, r13w
	cfcmovnl r12d, r14d
	cfcmovnl r11, r15
	cfcmovnl r12w, r13w
	cfcmovnl r18w, r28w
	cfcmovnl r26d, r22d
	cfcmovnl r28, r16
	cfcmovnl r17w, r19w
	cfcmovnl qword [eax+1], rdx
	cfcmovnl qword [eax+64], rdx
	cfcmovnl rdx, qword [eax+1]
	cfcmovnl rcx, qword [eax+64]
	cfcmovnl rdx, rcx, qword [eax+1]
	cfcmovnl rbp, rdx, qword [eax+64]
	cfcmovnl [0xc92], rdx
	cfcmovnl rcx, [0xb6f]
	cfcmovnl rdi, rbp, [0xedb]
%endif
