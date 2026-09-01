default rel
	vpshlq xmm1, xmm5, xmm4
	vpshlq xmm1, xmm4
	vpshlq xmm6, xmm3, xmm3
	vpshlq xmm6, xmm7, oword [0xec8]
	vpshlq xmm6, oword [0xec8]
	vpshlq xmm7, xmm1, oword [0x9ab]
	vpshlq xmm8, xmm10, xmm12
	vpshlq xmm14, xmm14, xmm10
	vpshlq xmm3, oword [eax+1], xmm1
	vpshlq xmm2, oword [eax+64], xmm7
	vpshlq xmm5, xmm4, oword [eax+1]
	vpshlq xmm4, xmm2, oword [eax+64]
	vpshlq xmm5, [0x506], xmm3
	vpshlq xmm7, xmm5, [0xe87]
