	vfmsub231bf16 xmm4, xmm2, xmm1
	vfmsub231bf16 xmm4, xmm4, xmm5
	vfmsub231bf16 ymm2, ymm5, ymm1
	vfmsub231bf16 ymm1, ymm3, ymm1
	vfmsub231bf16 zmm0, zmm3, zmm2
	vfmsub231bf16 zmm4, zmm0, zmm7

%ifdef ERROR
	vfmsub231bf16 xmm15, xmm9, xmm11
	vfmsub231bf16 ymm9, ymm9, ymm11
	vfmsub231bf16 zmm8, zmm10, zmm9
	vfmsub231bf16 xmm17, xmm24, xmm23
	vfmsub231bf16 ymm19, ymm22, ymm22
	vfmsub231bf16 zmm26, zmm26, zmm23
%endif
