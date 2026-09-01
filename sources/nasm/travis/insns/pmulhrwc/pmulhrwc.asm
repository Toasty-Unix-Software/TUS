default rel
	pmulhrwc mm2, mm4
	pmulhrwc mm1, mm6
	pmulhrwc mm5, qword [eax+1]
	pmulhrwc mm1, qword [eax+64]
	pmulhrwc mm1, [0x2f3]
