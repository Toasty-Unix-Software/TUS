	vfmadd213sh xmm5, xmm5, xmm2
	vfmadd213sh xmm5, xmm2
	vfmadd213sh xmm2, xmm3, xmm0

%ifdef ERROR
	vfmadd213sh xmm12, xmm10, xmm9
	vfmadd213sh xmm23, xmm29, xmm25
%endif
