default rel
	vrsqrt14sd xmm3, xmm0, qword [0xe52]
	vrsqrt14sd xmm3, qword [0xe52]
	vrsqrt14sd xmm6, xmm5, xmm1
	vrsqrt14sd xmm15, xmm13, xmm14
	vrsqrt14sd xmm21, xmm26, xmm21
	vrsqrt14sd xmm2{k1}, xmm5, xmm6
	vrsqrt14sd xmm5{k4}{z}, xmm2, xmm3
	vrsqrt14sd xmm4, xmm7, qword [eax+1]
	vrsqrt14sd xmm7, xmm7, qword [eax+64]
	vrsqrt14sd xmm2, xmm6, [0xc6d]
