	setp byte [0x74c]
	setp bl
	setp eax
	setp edi

%ifdef ERROR
	setp rbp
	setp rbx
	setp r14b
	setp r12
	setp r12d
	setp r25b
	setp r27
	setp r26d
%endif
