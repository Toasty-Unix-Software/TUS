default rel
	pfsubr mm5, mm6
	pfsubr mm5, qword [0x8f6]
	pfsubr mm0, qword [eax+1]
	pfsubr mm1, qword [eax+64]
	pfsubr mm4, [0xfde]
