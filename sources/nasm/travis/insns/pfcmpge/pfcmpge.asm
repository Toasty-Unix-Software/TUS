default rel
	pfcmpge mm7, qword [0x7b4]
	pfcmpge mm7, mm4
	pfcmpge mm5, qword [eax+1]
	pfcmpge mm0, qword [eax+64]
	pfcmpge mm7, [0x412]
