	hint_nop44 bx
	hint_nop44 word [0xb2d]
	hint_nop44 edx
	hint_nop44 dword [0x1f7]
	hint_nop44 qword [0x3b5]
	hint_nop44 qword [0x24d]

%ifdef ERROR
	hint_nop44 r10w
	hint_nop44 r14d
	hint_nop44 r15
	hint_nop44 r28w
	hint_nop44 r30d
	hint_nop44 r22
%endif
