default rel
	vrsqrt28ss xmm0, xmm5, dword [0x54e]
	vrsqrt28ss xmm0, dword [0x54e]
	vrsqrt28ss xmm3, xmm3, xmm4
	vrsqrt28ss xmm12, xmm12, xmm11
	vrsqrt28ss xmm26, xmm16, xmm29
	vrsqrt28ss xmm5{k4}, xmm7, dword [0x2e6]
	vrsqrt28ss xmm6{k6}{z}, xmm1, xmm7
	vrsqrt28ss xmm5, xmm2, xmm7, {sae}
	vrsqrt28ss xmm4, xmm5, dword [eax+1]
	vrsqrt28ss xmm6, xmm3, dword [eax+64]
	vrsqrt28ss xmm0, xmm1, [0xac3]
