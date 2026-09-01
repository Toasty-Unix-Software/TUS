default rel
	vtestpd xmm5, xmm4
	vtestpd xmm1, xmm0
	vtestpd ymm3, yword [0xcc7]
	vtestpd ymm3, ymm6
	vtestpd xmm11, xmm8
	vtestpd ymm14, ymm11
	vtestpd xmm6, oword [eax+1]
	vtestpd xmm7, oword [eax+64]
	vtestpd ymm5, yword [eax+1]
	vtestpd ymm1, yword [eax+64]
	vtestpd xmm6, [0x6db]
	vtestpd ymm6, [0xf9c]
