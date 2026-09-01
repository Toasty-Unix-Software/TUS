default rel
	pshufw mm4, qword [0x610], 0x33
	pshufw mm3, mm6, 0x3e
	pshufw mm6, qword [eax+1], 0x19
	pshufw mm2, qword [eax+64], 0x53
	pshufw mm6, [0x3a2], 0x78
