default rel
	psubsiw mm3, mm4
	psubsiw mm4, qword [0x4a3]
	psubsiw mm3, qword [eax+1]
	psubsiw mm1, qword [eax+64]
	psubsiw mm2, [0x10c]
