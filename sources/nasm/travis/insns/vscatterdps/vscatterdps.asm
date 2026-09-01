default rel
	vscatterdps [xmm4*1], xmm1
	vscatterdps [xmm3*1], xmm4
	vscatterdps [ymm6*1], ymm4
	vscatterdps [ymm5*1], ymm7
	vscatterdps [zmm7*1], zmm0
	vscatterdps [zmm7*1], zmm4
	vscatterdps [xmm8*1], xmm9
	vscatterdps [ymm14*1], ymm13
	vscatterdps [zmm13*1], zmm12
	vscatterdps [xmm25*1], xmm25
	vscatterdps [ymm25*1], ymm27
	vscatterdps [zmm30*1], zmm20
	vscatterdps [xmm0*1]{k1}, xmm2
	vscatterdps [ymm6*1]{k2}, ymm7
	vscatterdps [zmm4*1]{k5}, zmm5
