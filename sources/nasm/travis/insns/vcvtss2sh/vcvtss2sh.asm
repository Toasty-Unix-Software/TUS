default rel
	vcvtss2sh xmm4, xmm4, xmm2
	vcvtss2sh xmm4, xmm2
	vcvtss2sh xmm0, xmm4, xmm3
	vcvtss2sh xmm8, xmm15, xmm11
	vcvtss2sh xmm21, xmm25, xmm29
	vcvtss2sh xmm0, xmm7, xmm3, {rd-sae}
	vcvtss2sh xmm3, xmm6, dword [eax+1]
	vcvtss2sh xmm6, xmm0, dword [eax+64]
	vcvtss2sh xmm3, xmm7, [0xfdf]
