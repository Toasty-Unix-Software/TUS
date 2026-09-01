	encodekey128 edi, esi
	encodekey128 ebx, edi

%ifdef ERROR
	encodekey128 r14d, r12d
%endif
