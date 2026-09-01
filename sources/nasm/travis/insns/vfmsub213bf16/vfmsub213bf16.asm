default rel
	vfmsub213bf16 xmm5, xmm2, oword [0xa6d]
	vfmsub213bf16 xmm2, xmm4, xmm7
	vfmsub213bf16 ymm3, ymm5, yword [0x888]
	vfmsub213bf16 ymm2, ymm3, yword [0xc7c]
	vfmsub213bf16 zmm3, zmm4, zmm3
	vfmsub213bf16 zmm0, zmm3, zword [0x720]
	vfmsub213bf16 xmm11, xmm10, xmm8
	vfmsub213bf16 ymm14, ymm15, ymm8
	vfmsub213bf16 zmm14, zmm11, zmm13
	vfmsub213bf16 xmm25, xmm27, xmm29
	vfmsub213bf16 ymm28, ymm20, ymm20
	vfmsub213bf16 zmm24, zmm16, zmm25
	vfmsub213bf16 xmm1{k6}, xmm2, xmm1
	vfmsub213bf16 ymm2{k4}, ymm2, ymm3
	vfmsub213bf16 zmm4{k4}, zmm1, zmm0
	vfmsub213bf16 xmm1{k3}{z}, xmm1, xmm1
	vfmsub213bf16 ymm2{k4}{z}, ymm7, ymm0
	vfmsub213bf16 zmm4{k2}{z}, zmm6, zword [0x33c]
	vfmsub213bf16 xmm4, xmm5, oword [eax+1]
	vfmsub213bf16 xmm4, xmm6, oword [eax+64]
	vfmsub213bf16 ymm1, ymm1, yword [eax+1]
	vfmsub213bf16 ymm5, ymm4, yword [eax+64]
	vfmsub213bf16 zmm5, zmm0, zword [eax+1]
	vfmsub213bf16 zmm0, zmm5, zword [eax+64]
	vfmsub213bf16 xmm5, xmm6, [0x49f]
	vfmsub213bf16 ymm7, ymm1, [0xcb4]
	vfmsub213bf16 zmm3, zmm5, [0x514]
