default rel
	vrcp28sd xmm3, xmm4, qword [0x7c2]
	vrcp28sd xmm3, qword [0x7c2]
	vrcp28sd xmm6, xmm2, qword [0x675]
	vrcp28sd xmm10, xmm12, xmm15
	vrcp28sd xmm31, xmm27, xmm31
	vrcp28sd xmm2{k6}, xmm2, xmm3
	vrcp28sd xmm6{k7}{z}, xmm5, qword [0xaf7]
	vrcp28sd xmm0, xmm5, xmm1, {sae}
	vrcp28sd xmm6, xmm5, qword [eax+1]
	vrcp28sd xmm0, xmm1, qword [eax+64]
	vrcp28sd xmm7, xmm7, [0xd15]
