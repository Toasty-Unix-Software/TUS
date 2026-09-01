	vmovd xmm7, ecx
	vmovd xmm6, dword [0xc6e]
	vmovd dword [0x6e9], xmm2
	vmovd edi, xmm1
	vmovd xmm6, dword [0x50e]
	vmovd xmm1, xmm0
	vmovd xmm7, xmm4
	vmovd xmm4, xmm1

%ifdef ERROR
	vmovd xmm8, r9d
	vmovd r8d, xmm12
	vmovd xmm8, xmm11
	vmovd xmm13, xmm10
	vmovd xmm16, r18d
	vmovd r28d, xmm28
	vmovd xmm17, xmm21
	vmovd xmm20, xmm29
%endif
