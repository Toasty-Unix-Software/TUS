	vfnmsub231bf16 xmm7, xmm0, xmm6
	vfnmsub231bf16 xmm1, xmm1, xmm2
	vfnmsub231bf16 ymm1, ymm4, yword [0xb6e]
	vfnmsub231bf16 ymm5, ymm6, ymm0
	vfnmsub231bf16 zmm7, zmm6, zmm5
	vfnmsub231bf16 zmm0, zmm6, zword [0xee3]

%ifdef ERROR
	vfnmsub231bf16 xmm11, xmm14, xmm12
	vfnmsub231bf16 ymm14, ymm13, ymm10
	vfnmsub231bf16 zmm13, zmm8, zmm14
	vfnmsub231bf16 xmm25, xmm19, xmm26
	vfnmsub231bf16 ymm21, ymm26, ymm25
	vfnmsub231bf16 zmm30, zmm23, zmm30
%endif
