default rel
	vgetexpbf16 xmm4, xmm1
	vgetexpbf16 xmm5, xmm0
	vgetexpbf16 ymm0, yword [0x4a1]
	vgetexpbf16 ymm7, ymm1
	vgetexpbf16 zmm1, zword [0x5e1]
	vgetexpbf16 zmm2, zmm4
	vgetexpbf16 xmm10, xmm14
	vgetexpbf16 ymm10, ymm13
	vgetexpbf16 zmm14, zmm14
	vgetexpbf16 xmm29, xmm25
	vgetexpbf16 ymm20, ymm19
	vgetexpbf16 zmm17, zmm19
	vgetexpbf16 xmm5{k4}, oword [0xdf9]
	vgetexpbf16 ymm0{k6}, yword [0xd08]
	vgetexpbf16 zmm2{k3}, zmm2
	vgetexpbf16 xmm6{k1}{z}, xmm6
	vgetexpbf16 ymm4{k3}{z}, yword [0x505]
	vgetexpbf16 zmm6{k5}{z}, zword [0x32c]
	vgetexpbf16 xmm5, oword [eax+1]
	vgetexpbf16 xmm4, oword [eax+64]
	vgetexpbf16 ymm2, yword [eax+1]
	vgetexpbf16 ymm5, yword [eax+64]
	vgetexpbf16 zmm7, zword [eax+1]
	vgetexpbf16 zmm5, zword [eax+64]
	vgetexpbf16 xmm2, [0x3f5]
	vgetexpbf16 ymm3, [0x640]
	vgetexpbf16 zmm1, [0x8d3]
