default rel
	paddsiw mm2, qword [0x3dc]
	paddsiw mm4, mm3
	paddsiw mm3, qword [eax+1]
	paddsiw mm5, qword [eax+64]
	paddsiw mm5, [0x10f]
