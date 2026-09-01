.L1:
	call .L1
	call .L1
	call .L1
	call .L1
	call cx
	call word [0x3eb]

%ifdef ERROR
	call .L1
	call .L1
%endif
