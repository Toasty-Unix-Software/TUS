default rel
	vrndscalesd xmm5, xmm7, xmm7, 0x93
	vrndscalesd xmm5, xmm7, 0x93
	vrndscalesd xmm5, xmm0, qword [0x81b], 0x65
	vrndscalesd xmm13, xmm8, xmm9, 0x6a
	vrndscalesd xmm25, xmm19, xmm19, 0xfd
	vrndscalesd xmm4{k2}, xmm0, qword [0xc3d], 0xe2
	vrndscalesd xmm0{k6}{z}, xmm2, xmm5, 0xfe
	vrndscalesd xmm1, xmm0, qword [eax+1], 0x38
	vrndscalesd xmm0, xmm0, qword [eax+64], 0x8c
	vrndscalesd xmm7, xmm1, [0xece], 0x69
