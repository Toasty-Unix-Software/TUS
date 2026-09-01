default rel
	vucomisd xmm2, xmm4
	vucomisd xmm7, qword [0x8a8]
	vucomisd xmm2, qword [0x76f]
	vucomisd xmm0, xmm7
	vucomisd xmm13, xmm9
	vucomisd xmm15, xmm8
	vucomisd xmm30, xmm18
	vucomisd xmm23, xmm27
	vucomisd xmm0, xmm6, {sae}
	vucomisd xmm2, qword [eax+1]
	vucomisd xmm6, qword [eax+64]
	vucomisd xmm4, qword [eax+1]
	vucomisd xmm0, qword [eax+64]
	vucomisd xmm5, [0xbb9]
	vucomisd xmm1, [0x429]
