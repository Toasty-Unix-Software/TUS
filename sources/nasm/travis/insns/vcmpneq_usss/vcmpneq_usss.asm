default rel
	vcmpneq_usss xmm2, xmm6, xmm3
	vcmpneq_usss xmm2, xmm3
	vcmpneq_usss xmm4, xmm5, xmm2
	vcmpneq_usss k3, xmm6, xmm5
	vcmpneq_usss k4, xmm1, xmm6
	vcmpneq_usss xmm12, xmm8, xmm8
	vcmpneq_usss k2, xmm12, xmm12
	vcmpneq_usss k2{k4}, xmm6, xmm2
	vcmpneq_usss k7, xmm2, xmm2, {sae}
	vcmpneq_usss xmm4, xmm6, qword [eax+1]
	vcmpneq_usss xmm7, xmm2, qword [eax+64]
	vcmpneq_usss k3, xmm1, dword [eax+1]
	vcmpneq_usss k2, xmm6, dword [eax+64]
	vcmpneq_usss xmm1, xmm4, [0x203]
	vcmpneq_usss k6, xmm5, [0x662]
