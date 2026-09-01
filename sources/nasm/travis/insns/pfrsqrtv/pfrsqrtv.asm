default rel
	pfrsqrtv mm7, mm5
	pfrsqrtv mm2, mm1
	pfrsqrtv mm0, qword [eax+1]
	pfrsqrtv mm6, qword [eax+64]
	pfrsqrtv mm2, [0x124]
