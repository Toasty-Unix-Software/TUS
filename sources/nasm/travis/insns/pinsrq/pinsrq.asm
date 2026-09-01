default rel
	pinsrq xmm1, rbx, 0x5c
	pinsrq xmm1, rbx, 0x6a
	pinsrq xmm13, r15, 0x4c
	pinsrq xmm0, qword [eax+1], 0x56
	pinsrq xmm1, qword [eax+64], 0x6c
	pinsrq xmm3, [0x3ab], 0x27
