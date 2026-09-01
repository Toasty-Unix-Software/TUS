	movd mm7, edx
	movd mm4, ebx
	movd dword [0x96d], mm4
	movd esi, mm2
	movd mm5, rdi
	movd mm2, rcx
	movd rdi, mm2
	movd rsi, mm7

%ifdef ERROR
	movd mm7, r12d
	movd r12d, mm3
	movd mm2, r11
	movd r9, mm4
	movd mm0, r18d
	movd r19d, mm7
	movd mm3, r27
	movd r16, mm5
%endif
