	hint_nop29 dx
	hint_nop29 ax
	hint_nop29 eax
	hint_nop29 ebx
	hint_nop29 qword [0x4b9]
	hint_nop29 qword [0x529]

%ifdef ERROR
	hint_nop29 r13w
	hint_nop29 r14d
	hint_nop29 r10
	hint_nop29 r16w
	hint_nop29 r29d
	hint_nop29 r28
%endif
