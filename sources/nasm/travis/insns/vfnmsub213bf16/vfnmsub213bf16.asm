default rel
	vfnmsub213bf16 xmm6, xmm6, xmm7
	vfnmsub213bf16 xmm5, xmm3, xmm7
	vfnmsub213bf16 ymm6, ymm5, yword [0x58f]
	vfnmsub213bf16 ymm0, ymm1, ymm1
	vfnmsub213bf16 zmm7, zmm7, zword [0xd4e]
	vfnmsub213bf16 zmm3, zmm3, zword [0x5b8]
	vfnmsub213bf16 xmm13, xmm10, xmm10
	vfnmsub213bf16 ymm8, ymm11, ymm14
	vfnmsub213bf16 zmm11, zmm11, zmm9
	vfnmsub213bf16 xmm16, xmm30, xmm24
	vfnmsub213bf16 ymm30, ymm17, ymm28
	vfnmsub213bf16 zmm29, zmm25, zmm29
	vfnmsub213bf16 xmm0{k7}, xmm3, xmm7
	vfnmsub213bf16 ymm2{k7}, ymm0, ymm7
	vfnmsub213bf16 zmm6{k4}, zmm5, zword [0xd7c]
	vfnmsub213bf16 xmm1{k2}{z}, xmm4, oword [0x6d3]
	vfnmsub213bf16 ymm6{k5}{z}, ymm2, ymm1
	vfnmsub213bf16 zmm3{k5}{z}, zmm6, zword [0x14b]
	vfnmsub213bf16 xmm2, xmm6, oword [eax+1]
	vfnmsub213bf16 xmm2, xmm2, oword [eax+64]
	vfnmsub213bf16 ymm5, ymm2, yword [eax+1]
	vfnmsub213bf16 ymm3, ymm0, yword [eax+64]
	vfnmsub213bf16 zmm2, zmm5, zword [eax+1]
	vfnmsub213bf16 zmm5, zmm6, zword [eax+64]
	vfnmsub213bf16 xmm0, xmm2, [0xaf7]
	vfnmsub213bf16 ymm1, ymm5, [0xfcf]
	vfnmsub213bf16 zmm6, zmm2, [0x3a0]
