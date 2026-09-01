	vfnmadd213sd xmm5, xmm3, xmm1
	vfnmadd213sd xmm4, xmm5, xmm2
	vfnmadd213sd xmm5, xmm2, xmm1
	vfnmadd213sd xmm2, xmm1, qword [0xd15]

%ifdef ERROR
	vfnmadd213sd xmm12, xmm10, xmm9
	vfnmadd213sd xmm9, xmm15, xmm11
	vfnmadd213sd xmm21, xmm31, xmm19
	vfnmadd213sd xmm28, xmm31, xmm21
%endif
