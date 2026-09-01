	vfmadd231bf16 xmm6, xmm4, xmm2
	vfmadd231bf16 xmm0, xmm6, xmm6
	vfmadd231bf16 ymm5, ymm4, ymm5
	vfmadd231bf16 ymm6, ymm0, ymm5
	vfmadd231bf16 zmm2, zmm7, zmm6
	vfmadd231bf16 zmm4, zmm7, zmm6

%ifdef ERROR
	vfmadd231bf16 xmm10, xmm15, xmm11
	vfmadd231bf16 ymm10, ymm8, ymm8
	vfmadd231bf16 zmm11, zmm10, zmm11
	vfmadd231bf16 xmm30, xmm22, xmm31
	vfmadd231bf16 ymm31, ymm25, ymm27
	vfmadd231bf16 zmm16, zmm28, zmm22
%endif
