.L1:
	loopnz .L1
	loopnz .L1
	loopnz .L1
	loopnz .L1
	loopnz .L1, cx
	loopnz .L1, cx

%ifdef ERROR
	loopnz .L1
	loopnz .L1
%endif
