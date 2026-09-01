default rel
	ptwrite edi
	ptwrite dword [0x1de]
	ptwrite qword [0xa99]
	ptwrite qword [0x1ca]
	ptwrite r15d
	ptwrite r9
	ptwrite r24d
	ptwrite r28
	ptwrite dword [eax+1]
	ptwrite dword [eax+64]
	ptwrite qword [eax+1]
	ptwrite qword [eax+64]
	ptwrite [0x7ce]
	ptwrite [0x8fa]
