default rel
	vrsqrt28sd xmm0, xmm0, xmm0
	vrsqrt28sd xmm0, xmm0
	vrsqrt28sd xmm2, xmm3, xmm4
	vrsqrt28sd xmm11, xmm10, xmm12
	vrsqrt28sd xmm27, xmm27, xmm25
	vrsqrt28sd xmm5{k7}, xmm5, qword [0xa07]
	vrsqrt28sd xmm5{k6}{z}, xmm3, xmm0
	vrsqrt28sd xmm5, xmm5, xmm2, {sae}
	vrsqrt28sd xmm0, xmm2, qword [eax+1]
	vrsqrt28sd xmm7, xmm3, qword [eax+64]
	vrsqrt28sd xmm5, xmm1, [0x651]
