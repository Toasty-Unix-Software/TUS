default rel
	pfrcp mm3, mm0
	pfrcp mm3, qword [0xbb1]
	pfrcp mm0, qword [eax+1]
	pfrcp mm3, qword [eax+64]
	pfrcp mm7, [0x261]
