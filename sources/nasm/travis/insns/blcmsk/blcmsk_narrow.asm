	blcmsk ebx, ecx
	blcmsk edi, esi

%ifdef ERROR
	blcmsk rdi, rdi
	blcmsk rbx, qword [0x756]
	blcmsk r11d, r13d
	blcmsk r10, r13
	blcmsk rbx, qword [eax+1]
	blcmsk rbx, qword [eax+64]
	blcmsk rcx, [0xdf3]
%endif
