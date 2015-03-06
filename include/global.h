#if defined(SYS_OpenBSD)
# define n_strcpy(dst, src, dstsize) strlcpy(dst, src, dstsize)
# define n_strcat(dst, src, dstsize) strlcat(dst, src, dstsize)
#else
# define n_strcpy(dst, src, dstsize) strcpy(dst, src)
# define n_strcat(dst, src, dstsize) strcat(dst, src)
#endif
