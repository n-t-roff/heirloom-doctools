#if defined(SYS_OpenBSD) || \
    defined(SYS_NetBSD ) || \
    defined(SYS_FreeBSD)
# define n_strcpy(dst, src, dstsize) strlcpy(dst, src, dstsize)
# define n_strcat(dst, src, dstsize) strlcat(dst, src, dstsize)
# define n_wcscpy(dst, src, size   ) wcslcpy(dst, src, size   )
#else
# define n_strcpy(dst, src, dstsize) strcpy(dst, src)
# define n_strcat(dst, src, dstsize) strcat(dst, src)
# define n_wcscpy(dst, src, size   ) wcscpy(dst, src)
#endif
