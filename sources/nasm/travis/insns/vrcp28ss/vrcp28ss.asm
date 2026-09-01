default rel
	vrcp28ss xmm1, xmm3, xmm5
	vrcp28ss xmm1, xmm5
	vrcp28ss xmm3, xmm5, xmm7
	vrcp28ss xmm14, xmm15, xmm10
	vrcp28ss xmm26, xmm17, xmm23
	vrcp28ss xmm6{k2}, xmm0, dword [0xd5d]
	vrcp28ss xmm4{k1}{z}, xmm6, xmm1
	vrcp28ss xmm4, xmm4, xmm5, {sae}
	vrcp28ss xmm3, xmm6, dword [eax+1]
	vrcp28ss xmm2, xmm3, dword [eax+64]
	vrcp28ss xmm1, xmm5, [0xe77]
