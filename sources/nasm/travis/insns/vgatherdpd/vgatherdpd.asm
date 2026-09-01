default rel
	vgatherdpd xmm1, [xmm2*1], xmm1
	vgatherdpd xmm3, [xmm7*1], xmm6
	vgatherdpd ymm7, [xmm6*1], ymm7
	vgatherdpd ymm5, [xmm6*1], ymm6
	vgatherdpd xmm5, [xmm2*1]
	vgatherdpd xmm6, [xmm3*1]
	vgatherdpd ymm1, [xmm7*1]
	vgatherdpd ymm5, [xmm4*1]
	vgatherdpd xmm9, [xmm15*1], xmm12
	vgatherdpd ymm11, [xmm14*1], ymm14
	vgatherdpd xmm9, [xmm15*1]
	vgatherdpd ymm11, [xmm12*1]
	vgatherdpd xmm6{k3}, [xmm3*1]
	vgatherdpd ymm3{k4}, [xmm0*1]
	vgatherdpd zmm2{k1}, [ymm0*1]
