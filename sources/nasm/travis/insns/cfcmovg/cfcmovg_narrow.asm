	cfcmovg bp, bp
	cfcmovg word [0x919], ax
	cfcmovg dword [0xfbb], ebx
	cfcmovg eax, ebx
	cfcmovg di, di
	cfcmovg cx, word [0x286]

%ifdef ERROR
	cfcmovg rdx, rax
	cfcmovg rax, rbp
	cfcmovg r11w, r15w
	cfcmovg r15d, r8d
	cfcmovg r9, r8
	cfcmovg r8w, r15w
	cfcmovg r27w, r24w
	cfcmovg r31d, r25d
	cfcmovg r29, r31
	cfcmovg r27w, r27w
	cfcmovg qword [eax+1], rbx
	cfcmovg qword [eax+64], rdi
	cfcmovg rsi, qword [eax+1]
	cfcmovg rdx, qword [eax+64]
	cfcmovg rax, rsi, qword [eax+1]
	cfcmovg rsi, rsi, qword [eax+64]
	cfcmovg [0xfc8], rsi
	cfcmovg rbx, [0x951]
	cfcmovg rsi, rbx, [0x106]
%endif
