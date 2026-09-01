	imul byte [0xd2f]
	imul byte [0x565]
	imul bx
	imul si
	imul edx
	imul eax
	imul rdi
	imul rdx

%ifdef ERROR
	imul r13b
	imul r10w
	imul r11d
	imul r11
	imul r21b
	imul r31w
	imul r28d
	imul r25
	imul rdi, qword [eax+1]
	imul rdx, qword [eax+64]
	imul rcx, qword [eax+1], 52
	imul rdi, qword [eax+64], -34
	imul rsi, qword [eax+1], -105510694
	imul rbx, qword [eax+64], -378722242
	imul rbp, rcx, qword [eax+1]
	imul rcx, rdi, qword [eax+64]
	imul rdi, qword [eax+1], 75
	imul rdi, qword [eax+64], 46
	imul rdx, qword [eax+1], 287876982
	imul rdx, qword [eax+64], 371340418
	imul rdx, [0xa3c]
	imul rcx, [0x371], -114
	imul rdx, [0x992], 491107648
	imul rbx, rcx, [0x7e0]
	imul rbx, [0xcfc], 27
	imul rbp, [0xf3c], 258666374
%endif
