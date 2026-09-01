default rel
	vmovd xmm7, ecx
	vmovd xmm6, dword [0xc6e]
	vmovd dword [0x6e9], xmm2
	vmovd edi, xmm1
	vmovd xmm6, dword [0x50e]
	vmovd xmm1, xmm0
	vmovd xmm7, xmm4
	vmovd xmm4, xmm1
	vmovd xmm8, r9d
	vmovd r8d, xmm12
	vmovd xmm8, xmm11
	vmovd xmm13, xmm10
	vmovd xmm16, r18d
	vmovd r28d, xmm28
	vmovd xmm17, xmm21
	vmovd xmm20, xmm29
	vmovd xmm1, dword [eax+1]
	vmovd xmm0, dword [eax+64]
	vmovd dword [eax+1], xmm1
	vmovd dword [eax+64], xmm7
	vmovd xmm3, dword [eax+1]
	vmovd xmm1, dword [eax+64]
	vmovd dword [eax+1], xmm3
	vmovd dword [eax+64], xmm3
	vmovd xmm7, [0xa22]
	vmovd [0xcce], xmm0
	vmovd xmm3, [0x2b7]
	vmovd [0xa41], xmm1
