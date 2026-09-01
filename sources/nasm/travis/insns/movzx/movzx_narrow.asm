	movzx si, dl
	movzx bx, byte [0x756]
	movzx ebx, dl
	movzx edx, byte [0xc18]
	movzx ax, word [0x112]
	movzx ax, ax

%ifdef ERROR
	movzx rax, byte [0xa93]
	movzx rcx, byte [0x1d7]
	movzx r8w, r11b
	movzx r10d, r11b
	movzx r12, r12b
	movzx r10w, r12w
	movzx r16w, r16b
	movzx r26d, r31b
	movzx r26, r23b
	movzx r24w, r25w
%endif
