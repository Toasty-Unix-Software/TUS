	cmpoxadd dword [0x361], edx, esi
	cmpoxadd dword [0x675], ebx, esi

%ifdef ERROR
	cmpoxadd qword [0xbfb], rdx, rdx
	cmpoxadd qword [0xad2], rdi, rdx
	cmpoxadd dword [0x3c5], r11d, r9d
	cmpoxadd qword [0xb3b], r10, r15
	cmpoxadd dword [0x81d], r29d, r26d
	cmpoxadd qword [0xab5], r20, r23
	cmpoxadd qword [eax+1], rcx, rcx
	cmpoxadd qword [eax+64], rdi, rsi
	cmpoxadd [0x985], rdi, rbp
%endif
