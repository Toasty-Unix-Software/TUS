default rel
	vrcp14ss xmm4, xmm7, xmm4
	vrcp14ss xmm4, xmm4
	vrcp14ss xmm4, xmm2, xmm0
	vrcp14ss xmm14, xmm13, xmm15
	vrcp14ss xmm29, xmm28, xmm22
	vrcp14ss xmm7{k1}, xmm4, xmm1
	vrcp14ss xmm5{k3}{z}, xmm2, xmm0
	vrcp14ss xmm0, xmm0, dword [eax+1]
	vrcp14ss xmm7, xmm4, dword [eax+64]
	vrcp14ss xmm1, xmm4, [0x3ab]
