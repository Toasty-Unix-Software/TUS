	tzmsk ecx, ecx
	tzmsk eax, ebx

%ifdef ERROR
	tzmsk rcx, rdi
	tzmsk rdi, rbx
	tzmsk r11d, r12d
	tzmsk r9, r8
	tzmsk rbp, qword [eax+1]
	tzmsk rdx, qword [eax+64]
	tzmsk rbx, [0x928]
%endif
