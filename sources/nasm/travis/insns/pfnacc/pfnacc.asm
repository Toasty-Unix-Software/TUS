default rel
	pfnacc mm3, mm1
	pfnacc mm3, qword [0xcee]
	pfnacc mm3, qword [eax+1]
	pfnacc mm1, qword [eax+64]
	pfnacc mm0, [0x6dc]
