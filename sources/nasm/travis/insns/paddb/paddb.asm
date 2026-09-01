default rel
	paddb mm5, qword [0x841]
	paddb mm5, mm1
	paddb xmm5, xmm1
	paddb xmm1, xmm3
	paddb xmm8, xmm14
	paddb mm4, qword [eax+1]
	paddb mm5, qword [eax+64]
	paddb xmm5, oword [eax+1]
	paddb xmm1, oword [eax+64]
	paddb mm7, [0x7a0]
	paddb xmm7, [0xd6a]
