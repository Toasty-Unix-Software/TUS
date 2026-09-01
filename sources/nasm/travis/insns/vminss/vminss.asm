default rel
	vminss xmm3, xmm1, dword [0xafc]
	vminss xmm3, dword [0xafc]
	vminss xmm1, xmm5, dword [0x5dd]
	vminss xmm7, xmm6, xmm7
	vminss xmm7, xmm7
	vminss xmm2, xmm1, xmm5
	vminss xmm10, xmm11, xmm11
	vminss xmm10, xmm12, xmm11
	vminss xmm27, xmm21, xmm18
	vminss xmm17, xmm17, xmm23
	vminss xmm5{k6}, xmm3, xmm1
	vminss xmm1{k4}{z}, xmm3, xmm4
	vminss xmm2, xmm7, xmm7, {sae}
	vminss xmm0, xmm6, dword [eax+1]
	vminss xmm4, xmm7, dword [eax+64]
	vminss xmm4, xmm5, dword [eax+1]
	vminss xmm4, xmm6, dword [eax+64]
	vminss xmm2, xmm6, [0xd78]
	vminss xmm6, xmm0, [0x2b0]
