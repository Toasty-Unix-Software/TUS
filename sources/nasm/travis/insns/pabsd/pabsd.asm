default rel
	pabsd mm0, mm5
	pabsd mm0, mm0
	pabsd xmm1, xmm0
	pabsd xmm3, xmm4
	pabsd xmm12, xmm8
	pabsd mm4, qword [eax+1]
	pabsd mm4, qword [eax+64]
	pabsd xmm2, oword [eax+1]
	pabsd xmm4, oword [eax+64]
	pabsd mm3, [0xc4e]
	pabsd xmm3, [0xf23]
