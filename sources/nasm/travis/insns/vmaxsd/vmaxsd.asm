default rel
	vmaxsd xmm2, xmm6, xmm1
	vmaxsd xmm2, xmm1
	vmaxsd xmm4, xmm1, qword [0xe4e]
	vmaxsd xmm3, xmm6, qword [0x65c]
	vmaxsd xmm3, qword [0x65c]
	vmaxsd xmm1, xmm4, qword [0xe8a]
	vmaxsd xmm14, xmm14, xmm8
	vmaxsd xmm9, xmm14, xmm9
	vmaxsd xmm23, xmm27, xmm19
	vmaxsd xmm29, xmm16, xmm28
	vmaxsd xmm6{k3}, xmm0, xmm4
	vmaxsd xmm6{k2}{z}, xmm3, qword [0xb31]
	vmaxsd xmm7, xmm5, xmm4, {sae}
	vmaxsd xmm3, xmm7, qword [eax+1]
	vmaxsd xmm7, xmm5, qword [eax+64]
	vmaxsd xmm4, xmm4, qword [eax+1]
	vmaxsd xmm3, xmm4, qword [eax+64]
	vmaxsd xmm1, xmm7, [0x26d]
	vmaxsd xmm4, xmm2, [0xb57]
