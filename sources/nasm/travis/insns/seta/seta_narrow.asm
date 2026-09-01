	seta bl
	seta byte [0xcfa]
	seta ebx
	seta esi

%ifdef ERROR
	seta rsi
	seta rsi
	seta r10b
	seta r13
	seta r8d
	seta r24b
	seta r28
	seta r18d
%endif
