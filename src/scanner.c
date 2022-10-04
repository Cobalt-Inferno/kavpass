#include "scanner.h"
static bool isfile(char *file)
{
    struct stat s;
    if (stat(file,&s) == 0) {
        return true;
    }
    else {
        return false;
    }
    return false;
}
char *tmpbuff()
{
    int fp = open("/tmp/data", 0);
    struct stat st;
    if (fstat(fp,&st) == -1) {
        fprintf(stderr, "Unable to access data at: %s. Use `shuttle --update` to resolve.\n","/tmp/data");
    }
    char *buff = mmap(NULL, (size_t) st.st_size, PROT_READ, MAP_PRIVATE, fp, 0);
    return buff;
}
bool quickscan(const char *md5)
{
    printf("%d\n", O_RDONLY);
    if(strcmp(md5, "d41d8cd98f00b204e9800998ecf8427e") == 0)
        return false;
    if (!(isfile("/tmp/data"))) {
        fprintf(stderr, "Unable to access data at: %s. Use `shuttle --update` to resolve.\n","/tmp/data");
        exit(1);
    }
    FILE *fp = fopen("/tmp/data", "r");
    char buff[1024];
    while(fgets(buff, 1024, fp) != NULL) {
        if(strstr(buff, md5) != NULL) {
            // add function to add the flagged file to a linked list for removal
            return true;
        }
    }
    fclose(fp);
    return false;
}
char *loadbuf()
{
    char * buffer = 0;
    long length;
    if (!(isfile("/tmp/data"))) {
        fprintf(stderr,"Unable to access data at: %s. Use `shuttle --update` to resolve.\n","/tmp/data");
        exit(1);
    }
    FILE * f = fopen ("/tmp/data", "rb");
    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc ((long unsigned int)length);
        if (buffer)
        {
            fread (buffer, 1, (long unsigned int)length, f);
        }
        fclose (f);
    }
    if (buffer)
    {
        return buffer;
    }
    fclose(f);
    return 0;
}
bool testscan(const char *md5, const char *buf)
{
    if (strcmp(md5, "d41d8cd98f00b204e9800998ecf8427e") == 0)
        return false;
    if(strstr(buf, md5) != NULL) {
        return true;
    }
    return false;
}
