	hint_nop33 word [0x12f]
	hint_nop33 cx
	hint_nop33 eax
	hint_nop33 dword [0x2c7]
	hint_nop33 rcx
	hint_nop33 rcx

%ifdef ERROR
	hint_nop33 r14w
	hint_nop33 r8d
	hint_nop33 r11
	hint_nop33 r22w
	hint_nop33 r24d
	hint_nop33 r30
%endif
