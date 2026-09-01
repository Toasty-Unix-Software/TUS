	hint_nop31 cx
	hint_nop31 word [0xad3]
	hint_nop31 eax
	hint_nop31 ecx
	hint_nop31 qword [0x63b]
	hint_nop31 qword [0x3ce]

%ifdef ERROR
	hint_nop31 r15w
	hint_nop31 r15d
	hint_nop31 r9
	hint_nop31 r17w
	hint_nop31 r16d
	hint_nop31 r25
%endif
