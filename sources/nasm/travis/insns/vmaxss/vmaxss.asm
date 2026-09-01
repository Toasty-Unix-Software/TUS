default rel
	vmaxss xmm7, xmm6, xmm4
	vmaxss xmm7, xmm4
	vmaxss xmm0, xmm5, xmm5
	vmaxss xmm7, xmm6, xmm3
	vmaxss xmm7, xmm3
	vmaxss xmm0, xmm7, xmm0
	vmaxss xmm8, xmm11, xmm14
	vmaxss xmm13, xmm13, xmm12
	vmaxss xmm22, xmm27, xmm17
	vmaxss xmm16, xmm24, xmm26
	vmaxss xmm2{k1}, xmm5, dword [0xce4]
	vmaxss xmm6{k5}{z}, xmm1, xmm4
	vmaxss xmm2, xmm2, xmm2, {sae}
	vmaxss xmm7, xmm5, dword [eax+1]
	vmaxss xmm0, xmm6, dword [eax+64]
	vmaxss xmm7, xmm7, dword [eax+1]
	vmaxss xmm1, xmm1, dword [eax+64]
	vmaxss xmm3, xmm1, [0xdf1]
	vmaxss xmm4, xmm0, [0xd49]
