default rel
	mulss xmm3, dword [0x495]
	mulss xmm7, xmm7
	mulss xmm12, xmm10
	mulss xmm7, dword [eax+1]
	mulss xmm6, dword [eax+64]
	mulss xmm4, [0x6db]
