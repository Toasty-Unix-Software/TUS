	hint_nop9 word [0xa5b]
	hint_nop9 bp
	hint_nop9 edi
	hint_nop9 ebx
	hint_nop9 qword [0xea7]
	hint_nop9 rax

%ifdef ERROR
	hint_nop9 r10w
	hint_nop9 r11d
	hint_nop9 r11
	hint_nop9 r22w
	hint_nop9 r28d
	hint_nop9 r28
%endif
