	dec si
	dec dx
	dec ecx
	dec ebp
	dec cl
	dec al
	dec dx
	dec bx

%ifdef ERROR
	dec r13w
	dec r13d
	dec r9b
	dec r10w
	dec r20w
	dec r28d
	dec r19b
	dec r26w
	dec rbp, qword [eax+1]
	dec rbx, qword [eax+64]
	dec rcx, [0x671]
%endif
