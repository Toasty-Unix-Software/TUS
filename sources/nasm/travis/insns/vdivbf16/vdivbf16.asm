default rel
	vdivbf16 xmm4, xmm7, xmm1
	vdivbf16 xmm1, xmm5, xmm0
	vdivbf16 ymm5, ymm4, yword [0xdee]
	vdivbf16 ymm2, ymm3, ymm2
	vdivbf16 zmm4, zmm7, zmm5
	vdivbf16 zmm7, zmm7, zmm6
	vdivbf16 xmm10, xmm14, xmm11
	vdivbf16 ymm11, ymm12, ymm12
	vdivbf16 zmm14, zmm13, zmm10
	vdivbf16 xmm23, xmm31, xmm29
	vdivbf16 ymm23, ymm18, ymm16
	vdivbf16 zmm28, zmm30, zmm20
	vdivbf16 xmm5{k1}, xmm7, oword [0x149]
	vdivbf16 ymm7{k5}, ymm1, ymm7
	vdivbf16 zmm7{k4}, zmm2, zword [0xdfe]
	vdivbf16 xmm2{k6}{z}, xmm7, xmm2
	vdivbf16 ymm7{k4}{z}, ymm7, ymm4
	vdivbf16 zmm1{k6}{z}, zmm6, zmm3
	vdivbf16 xmm3, xmm1, oword [eax+1]
	vdivbf16 xmm3, xmm0, oword [eax+64]
	vdivbf16 ymm6, ymm6, yword [eax+1]
	vdivbf16 ymm1, ymm0, yword [eax+64]
	vdivbf16 zmm6, zmm6, zword [eax+1]
	vdivbf16 zmm6, zmm3, zword [eax+64]
	vdivbf16 xmm5, xmm5, [0x7e3]
	vdivbf16 ymm7, ymm5, [0xbc2]
	vdivbf16 zmm5, zmm6, [0xceb]
