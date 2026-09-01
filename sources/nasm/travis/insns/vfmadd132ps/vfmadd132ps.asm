default rel
	vfmadd132ps xmm1, xmm4, xmm3
	vfmadd132ps xmm7, xmm2, oword [0xa90]
	vfmadd132ps ymm2, ymm5, ymm4
	vfmadd132ps ymm0, ymm7, ymm5
	vfmadd132ps xmm3, xmm5, xmm7
	vfmadd132ps xmm4, xmm5, xmm2
	vfmadd132ps ymm6, ymm3, ymm5
	vfmadd132ps ymm5, ymm0, ymm6
	vfmadd132ps xmm8, xmm10, xmm12
	vfmadd132ps ymm13, ymm12, ymm13
	vfmadd132ps xmm11, xmm10, xmm10
	vfmadd132ps ymm15, ymm11, ymm10
	vfmadd132ps xmm25, xmm17, xmm27
	vfmadd132ps ymm25, ymm21, ymm19
	vfmadd132ps xmm23, xmm23, xmm28
	vfmadd132ps ymm16, ymm28, ymm25
	vfmadd132ps xmm6{k6}, xmm3, oword [0x9da]
	vfmadd132ps ymm3{k2}, ymm6, yword [0x197]
	vfmadd132ps zmm7{k6}, zmm3, zmm2
	vfmadd132ps xmm6{k3}{z}, xmm7, xmm4
	vfmadd132ps ymm3{k3}{z}, ymm7, yword [0x130]
	vfmadd132ps zmm1{k7}{z}, zmm7, zmm2
	vfmadd132ps zmm5, zmm4, zmm1, {ru-sae}
	vfmadd132ps xmm0, xmm3, oword [eax+1]
	vfmadd132ps xmm1, xmm0, oword [eax+64]
	vfmadd132ps ymm6, ymm0, yword [eax+1]
	vfmadd132ps ymm1, ymm2, yword [eax+64]
	vfmadd132ps xmm4, xmm6, oword [eax+1]
	vfmadd132ps xmm3, xmm7, oword [eax+64]
	vfmadd132ps ymm1, ymm4, yword [eax+1]
	vfmadd132ps ymm6, ymm3, yword [eax+64]
	vfmadd132ps zmm7, zmm4, zword [eax+1]
	vfmadd132ps zmm5, zmm3, zword [eax+64]
	vfmadd132ps xmm2, xmm7, [0x907]
	vfmadd132ps ymm0, ymm4, [0x732]
	vfmadd132ps xmm2, xmm2, [0x629]
	vfmadd132ps ymm0, ymm6, [0xaad]
	vfmadd132ps zmm5, zmm1, [0x7d8]
