	hint_nop56 word [0xe9c]
	hint_nop56 word [0x4ad]
	hint_nop56 edi
	hint_nop56 dword [0x9ba]
	hint_nop56 rdx
	hint_nop56 qword [0x59a]

%ifdef ERROR
	hint_nop56 r9w
	hint_nop56 r12d
	hint_nop56 r9
	hint_nop56 r28w
	hint_nop56 r23d
	hint_nop56 r17
%endif
