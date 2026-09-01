	hint_nop28 word [0xe18]
	hint_nop28 bp
	hint_nop28 dword [0xb8b]
	hint_nop28 edx
	hint_nop28 rbx
	hint_nop28 qword [0x173]

%ifdef ERROR
	hint_nop28 r9w
	hint_nop28 r10d
	hint_nop28 r9
	hint_nop28 r27w
	hint_nop28 r24d
	hint_nop28 r29
%endif
