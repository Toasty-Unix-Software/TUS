	pextrw edx, mm5, 0x30
	pextrw ebp, mm3, 0x7f
	pextrw eax, xmm5, 0x58
	pextrw ecx, xmm7, 0x4f
	pextrw ecx, xmm5, 0x25
	pextrw eax, xmm2, 0x76

%ifdef ERROR
	pextrw rcx, xmm0, 0x5
	pextrw rdx, xmm6, 0x27
	pextrw r8d, mm4, 0x7d
	pextrw r11d, xmm8, 0x53
	pextrw r14, xmm8, 0x14
	pextrw r8d, xmm11, 0x40
%endif
