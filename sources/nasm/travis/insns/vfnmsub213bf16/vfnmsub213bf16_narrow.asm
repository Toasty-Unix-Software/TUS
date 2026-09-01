	vfnmsub213bf16 xmm6, xmm6, xmm7
	vfnmsub213bf16 xmm5, xmm3, xmm7
	vfnmsub213bf16 ymm6, ymm5, yword [0x58f]
	vfnmsub213bf16 ymm0, ymm1, ymm1
	vfnmsub213bf16 zmm7, zmm7, zword [0xd4e]
	vfnmsub213bf16 zmm3, zmm3, zword [0x5b8]

%ifdef ERROR
	vfnmsub213bf16 xmm13, xmm10, xmm10
	vfnmsub213bf16 ymm8, ymm11, ymm14
	vfnmsub213bf16 zmm11, zmm11, zmm9
	vfnmsub213bf16 xmm16, xmm30, xmm24
	vfnmsub213bf16 ymm30, ymm17, ymm28
	vfnmsub213bf16 zmm29, zmm25, zmm29
%endif
