	bndcu bnd1, [0x482]
	bndcu bnd2, [0xb54]
	bndcu bnd3, edx
	bndcu bnd3, edx

%ifdef ERROR
	bndcu bnd1, rdx
	bndcu bnd0, rbx
%endif
