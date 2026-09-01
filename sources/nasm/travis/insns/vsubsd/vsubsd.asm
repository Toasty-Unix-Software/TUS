default rel
	vsubsd xmm6, xmm6, qword [0x11e]
	vsubsd xmm6, qword [0x11e]
	vsubsd xmm5, xmm0, xmm1
	vsubsd xmm4, xmm4, xmm1
	vsubsd xmm4, xmm1
	vsubsd xmm7, xmm3, xmm4
	vsubsd xmm10, xmm13, xmm11
	vsubsd xmm15, xmm14, xmm15
	vsubsd xmm30, xmm29, xmm30
	vsubsd xmm30, xmm16, xmm28
	vsubsd xmm1{k4}, xmm6, xmm1
	vsubsd xmm6{k3}{z}, xmm4, qword [0x91d]
	vsubsd xmm4, xmm6, xmm6, {rn-sae}
	vsubsd xmm1, xmm4, qword [eax+1]
	vsubsd xmm7, xmm6, qword [eax+64]
	vsubsd xmm6, xmm7, qword [eax+1]
	vsubsd xmm4, xmm4, qword [eax+64]
	vsubsd xmm6, xmm4, [0xc8a]
	vsubsd xmm0, xmm0, [0xef3]
