	vpextrw ebp, xmm2, 0x73
	vpextrw ecx, xmm6, 0xdb
	vpextrw word [0x45b], xmm4, 0xab
	vpextrw word [0x74e], xmm4, 0x77
	vpextrw dx, xmm5, 0xb0
	vpextrw cx, xmm7, 0xf0

%ifdef ERROR
	vpextrw rax, xmm5, 0xa6
	vpextrw rdi, xmm3, 0xd
	vpextrw r12, xmm15, 0xdc
	vpextrw r14d, xmm8, 0x1d
	vpextrw word [0x619], xmm10, 0x15
	vpextrw r10w, xmm9, 0xa9
	vpextrw r25, xmm24, 0x67
	vpextrw r22d, xmm28, 0xd9
	vpextrw word [0x9d0], xmm25, 0xc7
	vpextrw r16w, xmm22, 0x85
%endif
