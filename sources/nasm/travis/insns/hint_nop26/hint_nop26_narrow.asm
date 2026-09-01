	hint_nop26 bp
	hint_nop26 di
	hint_nop26 ebp
	hint_nop26 ebp
	hint_nop26 rdi
	hint_nop26 qword [0xada]

%ifdef ERROR
	hint_nop26 r8w
	hint_nop26 r9d
	hint_nop26 r13
	hint_nop26 r18w
	hint_nop26 r31d
	hint_nop26 r26
%endif
