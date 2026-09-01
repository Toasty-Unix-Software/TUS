default rel
	vdivss xmm6, xmm4, xmm6
	vdivss xmm6, xmm6
	vdivss xmm0, xmm6, xmm2
	vdivss xmm1, xmm4, xmm6
	vdivss xmm1, xmm6
	vdivss xmm6, xmm3, xmm1
	vdivss xmm11, xmm14, xmm12
	vdivss xmm12, xmm15, xmm10
	vdivss xmm29, xmm22, xmm26
	vdivss xmm21, xmm30, xmm27
	vdivss xmm0{k1}, xmm3, xmm5
	vdivss xmm2{k3}{z}, xmm6, dword [0x4ff]
	vdivss xmm5, xmm0, xmm7, {rz-sae}
	vdivss xmm2, xmm3, dword [eax+1]
	vdivss xmm0, xmm0, dword [eax+64]
	vdivss xmm4, xmm4, dword [eax+1]
	vdivss xmm4, xmm0, dword [eax+64]
	vdivss xmm7, xmm0, [0x6cd]
	vdivss xmm2, xmm7, [0xbdb]
