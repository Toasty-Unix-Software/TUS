default rel
	vgatherqps xmm7, [xmm4*1], xmm2
	vgatherqps xmm5, [xmm3*1], xmm5
	vgatherqps xmm2, [ymm7*1], xmm7
	vgatherqps xmm6, [ymm4*1], xmm2
	vgatherqps xmm1, [xmm0*1]
	vgatherqps xmm1, [xmm1*1]
	vgatherqps xmm1, [ymm7*1]
	vgatherqps xmm0, [ymm2*1]
	vgatherqps xmm10, [xmm14*1], xmm11
	vgatherqps xmm12, [ymm9*1], xmm14
	vgatherqps xmm12, [xmm12*1]
	vgatherqps xmm8, [ymm12*1]
	vgatherqps xmm2{k2}, [xmm5*1]
	vgatherqps xmm3{k6}, [ymm0*1]
	vgatherqps ymm2{k5}, [zmm7*1]
