default rel
	vscalefss xmm1, xmm3, dword [0xe70]
	vscalefss xmm1, dword [0xe70]
	vscalefss xmm5, xmm4, xmm3
	vscalefss xmm11, xmm8, xmm9
	vscalefss xmm26, xmm23, xmm29
	vscalefss xmm4{k1}, xmm0, xmm0
	vscalefss xmm5{k2}{z}, xmm0, dword [0x458]
	vscalefss xmm3, xmm1, xmm6, {rd-sae}
	vscalefss xmm5, xmm1, dword [eax+1]
	vscalefss xmm0, xmm0, dword [eax+64]
	vscalefss xmm3, xmm6, [0x481]
