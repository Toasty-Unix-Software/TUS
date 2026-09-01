	hint_nop42 word [0x479]
	hint_nop42 dx
	hint_nop42 edi
	hint_nop42 esi
	hint_nop42 qword [0x9f1]
	hint_nop42 rbp

%ifdef ERROR
	hint_nop42 r11w
	hint_nop42 r13d
	hint_nop42 r10
	hint_nop42 r16w
	hint_nop42 r27d
	hint_nop42 r22
%endif
