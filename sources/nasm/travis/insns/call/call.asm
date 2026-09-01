default rel
.L1:
	call .L1
	call .L1
	call .L1
	call .L1
	call .L1
	call .L1
	call [0xb37]
	call [0xe48]
	call [0xc6f]
	call [0x205]
	call [0x974]

%ifdef ERROR
	call cx
	call word [0x3eb]
%endif
