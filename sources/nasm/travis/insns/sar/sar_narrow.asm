	sar al, 1
	sar bl, 1
	sar word [0x726], 1
	sar ax, 1
	sar eax, 1
	sar dword [0xa7d], 1
	sar rbp, 1
	sar qword [0x69b], 1

%ifdef ERROR
	sar r11b, 1
	sar r9w, 1
	sar r12d, 1
	sar r10, 1
	sar r25b, 1
	sar r28w, 1
	sar r26d, 1
	sar r16, 1
%endif
