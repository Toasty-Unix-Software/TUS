default rel
	vmulss xmm4, xmm2, xmm0
	vmulss xmm4, xmm0
	vmulss xmm5, xmm6, xmm4
	vmulss xmm2, xmm1, dword [0xecc]
	vmulss xmm2, dword [0xecc]
	vmulss xmm0, xmm4, xmm1
	vmulss xmm8, xmm9, xmm14
	vmulss xmm14, xmm13, xmm14
	vmulss xmm28, xmm29, xmm19
	vmulss xmm30, xmm25, xmm31
	vmulss xmm4{k2}, xmm6, xmm6
	vmulss xmm3{k6}{z}, xmm6, xmm3
	vmulss xmm4, xmm7, xmm1, {rn-sae}
	vmulss xmm2, xmm6, dword [eax+1]
	vmulss xmm2, xmm0, dword [eax+64]
	vmulss xmm4, xmm7, dword [eax+1]
	vmulss xmm3, xmm3, dword [eax+64]
	vmulss xmm0, xmm3, [0xbed]
	vmulss xmm4, xmm3, [0xaf8]
