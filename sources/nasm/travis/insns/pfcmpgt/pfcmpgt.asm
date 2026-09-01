default rel
	pfcmpgt mm3, mm2
	pfcmpgt mm6, mm4
	pfcmpgt mm2, qword [eax+1]
	pfcmpgt mm1, qword [eax+64]
	pfcmpgt mm2, [0x986]
