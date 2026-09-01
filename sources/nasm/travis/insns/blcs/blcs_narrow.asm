	blcs ecx, ebx
	blcs esi, edx

%ifdef ERROR
	blcs rbx, rsi
	blcs rbp, qword [0x9a2]
	blcs r15d, r9d
	blcs r11, r10
	blcs rcx, qword [eax+1]
	blcs rbx, qword [eax+64]
	blcs rdx, [0xc3c]
%endif
