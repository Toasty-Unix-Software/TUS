	vfmsub213bf16 xmm5, xmm2, oword [0xa6d]
	vfmsub213bf16 xmm2, xmm4, xmm7
	vfmsub213bf16 ymm3, ymm5, yword [0x888]
	vfmsub213bf16 ymm2, ymm3, yword [0xc7c]
	vfmsub213bf16 zmm3, zmm4, zmm3
	vfmsub213bf16 zmm0, zmm3, zword [0x720]

%ifdef ERROR
	vfmsub213bf16 xmm11, xmm10, xmm8
	vfmsub213bf16 ymm14, ymm15, ymm8
	vfmsub213bf16 zmm14, zmm11, zmm13
	vfmsub213bf16 xmm25, xmm27, xmm29
	vfmsub213bf16 ymm28, ymm20, ymm20
	vfmsub213bf16 zmm24, zmm16, zmm25
%endif
