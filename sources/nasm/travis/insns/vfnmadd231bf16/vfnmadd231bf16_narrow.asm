	vfnmadd231bf16 xmm5, xmm4, xmm2
	vfnmadd231bf16 xmm4, xmm2, xmm2
	vfnmadd231bf16 ymm7, ymm2, ymm4
	vfnmadd231bf16 ymm1, ymm5, ymm3
	vfnmadd231bf16 zmm1, zmm0, zword [0x3a7]
	vfnmadd231bf16 zmm7, zmm6, zword [0x64e]

%ifdef ERROR
	vfnmadd231bf16 xmm9, xmm10, xmm13
	vfnmadd231bf16 ymm15, ymm13, ymm9
	vfnmadd231bf16 zmm15, zmm10, zmm15
	vfnmadd231bf16 xmm28, xmm22, xmm23
	vfnmadd231bf16 ymm29, ymm17, ymm22
	vfnmadd231bf16 zmm27, zmm25, zmm27
%endif
