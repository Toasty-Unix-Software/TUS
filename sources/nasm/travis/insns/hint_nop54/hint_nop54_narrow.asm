	hint_nop54 bp
	hint_nop54 di
	hint_nop54 esi
	hint_nop54 edx
	hint_nop54 rdi
	hint_nop54 qword [0xae8]

%ifdef ERROR
	hint_nop54 r12w
	hint_nop54 r15d
	hint_nop54 r12
	hint_nop54 r17w
	hint_nop54 r24d
	hint_nop54 r24
%endif
