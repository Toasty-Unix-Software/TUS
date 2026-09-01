	hint_nop32 word [0xf3f]
	hint_nop32 bx
	hint_nop32 edi
	hint_nop32 ebx
	hint_nop32 rdx
	hint_nop32 rdx

%ifdef ERROR
	hint_nop32 r10w
	hint_nop32 r8d
	hint_nop32 r15
	hint_nop32 r16w
	hint_nop32 r27d
	hint_nop32 r18
%endif
