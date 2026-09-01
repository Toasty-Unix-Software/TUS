	vfnmsub132bf16 xmm5, xmm0, xmm4
	vfnmsub132bf16 xmm2, xmm4, xmm5
	vfnmsub132bf16 ymm2, ymm3, ymm4
	vfnmsub132bf16 ymm4, ymm0, ymm5
	vfnmsub132bf16 zmm7, zmm6, zword [0x819]
	vfnmsub132bf16 zmm6, zmm6, zword [0xaee]

%ifdef ERROR
	vfnmsub132bf16 xmm11, xmm8, xmm13
	vfnmsub132bf16 ymm14, ymm13, ymm14
	vfnmsub132bf16 zmm8, zmm9, zmm14
	vfnmsub132bf16 xmm16, xmm24, xmm19
	vfnmsub132bf16 ymm19, ymm26, ymm28
	vfnmsub132bf16 zmm31, zmm19, zmm20
%endif
