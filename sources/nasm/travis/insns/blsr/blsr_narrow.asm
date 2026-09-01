	blsr ecx, dword [0x4c6]
	blsr ecx, dword [0x8cd]

%ifdef ERROR
	blsr rdi, rcx
	blsr rbp, qword [0x32b]
	blsr r12d, r11d
	blsr r9, r11
	blsr r29d, r26d
	blsr r28, r26
	blsr rbp, qword [eax+1]
	blsr rbx, qword [eax+64]
	blsr rbp, [0x131]
%endif
