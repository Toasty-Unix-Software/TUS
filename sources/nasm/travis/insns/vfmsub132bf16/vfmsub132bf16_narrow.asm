	vfmsub132bf16 xmm0, xmm5, xmm1
	vfmsub132bf16 xmm2, xmm6, xmm7
	vfmsub132bf16 ymm7, ymm5, ymm6
	vfmsub132bf16 ymm4, ymm5, yword [0x2b9]
	vfmsub132bf16 zmm1, zmm7, zword [0x2f9]
	vfmsub132bf16 zmm1, zmm7, zmm1

%ifdef ERROR
	vfmsub132bf16 xmm11, xmm12, xmm10
	vfmsub132bf16 ymm11, ymm9, ymm13
	vfmsub132bf16 zmm10, zmm11, zmm9
	vfmsub132bf16 xmm17, xmm25, xmm21
	vfmsub132bf16 ymm19, ymm28, ymm30
	vfmsub132bf16 zmm26, zmm26, zmm31
%endif
