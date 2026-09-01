	ptwrite edi
	ptwrite dword [0x1de]
	ptwrite qword [0xa99]
	ptwrite qword [0x1ca]

%ifdef ERROR
	ptwrite r15d
	ptwrite r9
	ptwrite r24d
	ptwrite r28
%endif
