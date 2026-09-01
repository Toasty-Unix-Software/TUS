default rel
	pmulhriw mm0, mm0
	pmulhriw mm5, mm5
	pmulhriw mm3, qword [eax+1]
	pmulhriw mm0, qword [eax+64]
	pmulhriw mm3, [0x2db]
