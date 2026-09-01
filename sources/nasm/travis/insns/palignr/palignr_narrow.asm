	palignr mm5, mm3, 0x6c
	palignr mm3, mm2, 0x18
	palignr xmm4, xmm6, 0xa
	palignr xmm7, oword [0xaea], 0x7c

%ifdef ERROR
	palignr xmm8, xmm9, 0x15
%endif
