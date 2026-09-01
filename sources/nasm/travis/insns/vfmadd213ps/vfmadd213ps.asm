default rel
	vfmadd213ps xmm2, xmm6, oword [0x7c0]
	vfmadd213ps xmm2, xmm6, xmm2
	vfmadd213ps ymm3, ymm5, ymm3
	vfmadd213ps ymm0, ymm7, ymm4
	vfmadd213ps xmm2, xmm6, xmm5
	vfmadd213ps xmm6, xmm2, xmm4
	vfmadd213ps ymm3, ymm5, yword [0xf57]
	vfmadd213ps ymm1, ymm5, ymm0
	vfmadd213ps xmm9, xmm10, xmm10
	vfmadd213ps ymm15, ymm12, ymm15
	vfmadd213ps xmm10, xmm12, xmm15
	vfmadd213ps ymm12, ymm12, ymm14
	vfmadd213ps xmm25, xmm25, xmm21
	vfmadd213ps ymm18, ymm16, ymm31
	vfmadd213ps xmm30, xmm22, xmm25
	vfmadd213ps ymm20, ymm18, ymm26
	vfmadd213ps xmm2{k4}, xmm1, oword [0x42d]
	vfmadd213ps ymm6{k7}, ymm1, ymm5
	vfmadd213ps zmm4{k5}, zmm2, zword [0xbc3]
	vfmadd213ps xmm6{k7}{z}, xmm4, oword [0xcc0]
	vfmadd213ps ymm1{k2}{z}, ymm5, ymm7
	vfmadd213ps zmm0{k3}{z}, zmm0, zmm6
	vfmadd213ps zmm0, zmm7, zmm3, {rd-sae}
	vfmadd213ps xmm6, xmm4, oword [eax+1]
	vfmadd213ps xmm4, xmm1, oword [eax+64]
	vfmadd213ps ymm7, ymm1, yword [eax+1]
	vfmadd213ps ymm6, ymm0, yword [eax+64]
	vfmadd213ps xmm5, xmm2, oword [eax+1]
	vfmadd213ps xmm3, xmm3, oword [eax+64]
	vfmadd213ps ymm2, ymm2, yword [eax+1]
	vfmadd213ps ymm4, ymm4, yword [eax+64]
	vfmadd213ps zmm4, zmm0, zword [eax+1]
	vfmadd213ps zmm7, zmm7, zword [eax+64]
	vfmadd213ps xmm3, xmm1, [0x7c7]
	vfmadd213ps ymm1, ymm5, [0x29e]
	vfmadd213ps xmm2, xmm2, [0x785]
	vfmadd213ps ymm2, ymm4, [0xfbc]
	vfmadd213ps zmm0, zmm6, [0x3aa]
