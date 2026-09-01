	hint_nop53 cx
	hint_nop53 di
	hint_nop53 dword [0x3e9]
	hint_nop53 eax
	hint_nop53 qword [0x87d]
	hint_nop53 rcx

%ifdef ERROR
	hint_nop53 r10w
	hint_nop53 r14d
	hint_nop53 r8
	hint_nop53 r31w
	hint_nop53 r31d
	hint_nop53 r22
%endif
