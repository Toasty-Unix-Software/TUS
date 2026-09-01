	hint_nop12 cx
	hint_nop12 word [0x924]
	hint_nop12 ebp
	hint_nop12 eax
	hint_nop12 rax
	hint_nop12 rdx

%ifdef ERROR
	hint_nop12 r8w
	hint_nop12 r15d
	hint_nop12 r15
	hint_nop12 r26w
	hint_nop12 r17d
	hint_nop12 r27
%endif
