default rel
	vscalefbf16 xmm6, xmm6, oword [0xfbc]
	vscalefbf16 xmm4, xmm5, xmm3
	vscalefbf16 ymm6, ymm0, ymm6
	vscalefbf16 ymm1, ymm3, ymm4
	vscalefbf16 zmm5, zmm4, zword [0x422]
	vscalefbf16 zmm1, zmm0, zword [0xb1c]
	vscalefbf16 xmm13, xmm8, xmm12
	vscalefbf16 ymm9, ymm13, ymm10
	vscalefbf16 zmm11, zmm14, zmm8
	vscalefbf16 xmm27, xmm29, xmm28
	vscalefbf16 ymm18, ymm30, ymm23
	vscalefbf16 zmm18, zmm19, zmm17
	vscalefbf16 xmm1{k4}, xmm0, xmm3
	vscalefbf16 ymm4{k7}, ymm4, ymm2
	vscalefbf16 zmm5{k4}, zmm2, zmm7
	vscalefbf16 xmm6{k5}{z}, xmm4, xmm3
	vscalefbf16 ymm3{k5}{z}, ymm1, ymm5
	vscalefbf16 zmm2{k7}{z}, zmm5, zmm0
	vscalefbf16 xmm2, xmm7, oword [eax+1]
	vscalefbf16 xmm6, xmm4, oword [eax+64]
	vscalefbf16 ymm6, ymm3, yword [eax+1]
	vscalefbf16 ymm7, ymm0, yword [eax+64]
	vscalefbf16 zmm0, zmm6, zword [eax+1]
	vscalefbf16 zmm6, zmm7, zword [eax+64]
	vscalefbf16 xmm5, xmm5, [0x38a]
	vscalefbf16 ymm5, ymm4, [0xc8e]
	vscalefbf16 zmm5, zmm5, [0xb97]
