default rel
	vfnmsub231ss xmm7, xmm5, xmm5
	vfnmsub231ss xmm1, xmm0, xmm1
	vfnmsub231ss xmm4, xmm6, xmm2
	vfnmsub231ss xmm5, xmm7, xmm4
	vfnmsub231ss xmm10, xmm13, xmm9
	vfnmsub231ss xmm10, xmm11, xmm15
	vfnmsub231ss xmm17, xmm24, xmm29
	vfnmsub231ss xmm25, xmm21, xmm30
	vfnmsub231ss xmm1{k1}, xmm1, dword [0x4ce]
	vfnmsub231ss xmm3{k1}{z}, xmm1, xmm7
	vfnmsub231ss xmm1, xmm4, xmm7, {rz-sae}
	vfnmsub231ss xmm0, xmm5, dword [eax+1]
	vfnmsub231ss xmm3, xmm4, dword [eax+64]
	vfnmsub231ss xmm4, xmm5, dword [eax+1]
	vfnmsub231ss xmm2, xmm3, dword [eax+64]
	vfnmsub231ss xmm5, xmm2, [0xca6]
	vfnmsub231ss xmm0, xmm3, [0x675]
