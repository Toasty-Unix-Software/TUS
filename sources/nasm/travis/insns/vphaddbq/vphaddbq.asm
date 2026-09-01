default rel
	vphaddbq xmm5, oword [0xbf9]
	vphaddbq xmm5
	vphaddbq xmm5, oword [0xaff]
	vphaddbq xmm8, xmm8
	vphaddbq xmm2, oword [eax+1]
	vphaddbq xmm0, oword [eax+64]
	vphaddbq xmm6, [0xd87]
