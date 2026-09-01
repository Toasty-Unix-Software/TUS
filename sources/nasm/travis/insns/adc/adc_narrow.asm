	adc al, bl
	adc bl, dl
	adc di, dx
	adc bp, bp
	adc eax, ecx
	adc ebx, ecx

%ifdef ERROR
	adc rax, rdi
	adc qword [0x926], rsi
	adc r15b, r8b
	adc r11w, r9w
	adc r11d, r14d
	adc r9, r13
	adc r28b, r16b
	adc r16w, r27w
	adc r17d, r24d
	adc r26, r16
	adc qword [eax+1], rsi
	adc qword [eax+64], rbx
	adc rdi, qword [eax+1]
	adc rdx, qword [eax+64]
	adc qword [eax+1], -25
	adc qword [eax+64], -121
	adc qword [eax+1], -506139423
	adc qword [eax+64], -173191687
	adc rax, rax, qword [eax+1]
	adc rdx, rdx, qword [eax+64]
	adc rdi, qword [eax+1], rdx
	adc rdi, qword [eax+64], rsi
	adc rdi, qword [eax+1], 24
	adc rsi, qword [eax+64], -73
	adc rsi, qword [eax+1], -428886289
	adc rsi, qword [eax+64], 6139295
	adc [0x31c], rdx
	adc rdx, [0x342]
	adc [0x92a], -90
	adc [0x7a2], 41191012
	adc rbp, rbp, [0x4f1]
	adc rbp, [0x906], rbp
	adc rdx, [0x7d0], 59
	adc rdi, [0x7e4], 220977984
%endif
