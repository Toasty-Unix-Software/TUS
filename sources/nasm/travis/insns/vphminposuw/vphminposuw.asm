default rel
	vphminposuw xmm4, xmm5
	vphminposuw xmm6, xmm1
	vphminposuw xmm10, xmm12
	vphminposuw xmm6, oword [eax+1]
	vphminposuw xmm7, oword [eax+64]
	vphminposuw xmm4, [0xb29]
