	hint_nop25 bp
	hint_nop25 word [0xc82]
	hint_nop25 edi
	hint_nop25 ebx
	hint_nop25 rcx
	hint_nop25 rbx

%ifdef ERROR
	hint_nop25 r15w
	hint_nop25 r11d
	hint_nop25 r14
	hint_nop25 r16w
	hint_nop25 r16d
	hint_nop25 r26
%endif
