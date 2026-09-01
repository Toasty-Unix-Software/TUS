	vfmadd213ps xmm2, xmm6, oword [0x7c0]
	vfmadd213ps xmm2, xmm6, xmm2
	vfmadd213ps ymm3, ymm5, ymm3
	vfmadd213ps ymm0, ymm7, ymm4
	vfmadd213ps xmm2, xmm6, xmm5
	vfmadd213ps xmm6, xmm2, xmm4
	vfmadd213ps ymm3, ymm5, yword [0xf57]
	vfmadd213ps ymm1, ymm5, ymm0

%ifdef ERROR
	vfmadd213ps xmm9, xmm10, xmm10
	vfmadd213ps ymm15, ymm12, ymm15
	vfmadd213ps xmm10, xmm12, xmm15
	vfmadd213ps ymm12, ymm12, ymm14
	vfmadd213ps xmm25, xmm25, xmm21
	vfmadd213ps ymm18, ymm16, ymm31
	vfmadd213ps xmm30, xmm22, xmm25
	vfmadd213ps ymm20, ymm18, ymm26
%endif
