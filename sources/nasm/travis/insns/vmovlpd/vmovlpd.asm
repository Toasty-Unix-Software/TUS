default rel
	vmovlpd xmm7, xmm4, qword [0xd8e]
	vmovlpd xmm7, qword [0xd8e]
	vmovlpd xmm2, xmm0, qword [0xff0]
	vmovlpd qword [0x7a3], xmm0
	vmovlpd qword [0x5f7], xmm6
	vmovlpd xmm14, xmm9, qword [0x128]
	vmovlpd qword [0xff6], xmm10
	vmovlpd xmm27, xmm25, qword [0x3b4]
	vmovlpd qword [0x360], xmm21
	vmovlpd xmm2, xmm5, qword [eax+1]
	vmovlpd xmm5, xmm6, qword [eax+64]
	vmovlpd qword [eax+1], xmm6
	vmovlpd qword [eax+64], xmm6
	vmovlpd xmm6, xmm6, [0x491]
	vmovlpd [0xa34], xmm2
