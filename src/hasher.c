#include "hasher.h"
char *md5sum(char *filename)
{
    unsigned char cc[MD5_DIGEST_LENGTH], data[1024];
    size_t i, bytes;
    MD5_CTX context;
    char *md5 = malloc(1024 * sizeof(char));
    if (!md5) {
        fprintf(stderr,"Malloc failed. Aborting!\n");
    }
    FILE *ff = fopen(filename, "rb");
    if (!ff) {
        perror(filename);
        return filename;
    }
    MD5_Init(&context);
    while ((bytes = fread(data, 1, 1024, ff)) != 0) {
        MD5_Update(&context, data, bytes);
    }
    MD5_Final(cc, &context);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&md5[i*2], "%02x", cc[i]);
    }
    fclose(ff);
    return md5;
}

