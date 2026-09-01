default rel
	vgatherqpd xmm7, [xmm0*1], xmm0
	vgatherqpd xmm1, [xmm3*1], xmm5
	vgatherqpd ymm6, [ymm3*1], ymm7
	vgatherqpd ymm7, [ymm5*1], ymm1
	vgatherqpd xmm5, [xmm6*1]
	vgatherqpd xmm4, [xmm2*1]
	vgatherqpd ymm0, [ymm6*1]
	vgatherqpd ymm7, [ymm0*1]
	vgatherqpd xmm8, [xmm9*1], xmm11
	vgatherqpd ymm8, [ymm12*1], ymm12
	vgatherqpd xmm14, [xmm11*1]
	vgatherqpd ymm15, [ymm14*1]
	vgatherqpd xmm4{k2}, [xmm3*1]
	vgatherqpd ymm5{k3}, [ymm7*1]
	vgatherqpd zmm4{k5}, [zmm7*1]
