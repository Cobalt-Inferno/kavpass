#include "top_level_scan.h"
void strip_extra_spaces(char* str) {
    int i, x;
    for(i=x=0; str[i]; ++i)
        if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1])))
            str[x++] = str[i];
    str[x] = '\0';
}
bool isdir(char *path) {
    struct stat s;
    if( stat(path,&s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
        {
            return true;
        }
        else if( s.st_mode & S_IFREG )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
    else
    {
        perror("stat");
    }
    return false;
}
void GetFiles(char *name, int indent, bool testing, char *tbuf)
{
    DIR *dir;
    struct dirent *entry;
    char *newName;
    newName = strdup(name);
    size_t len = strlen(newName);
    if (newName[len - 1] == '/')
        newName[len - 1] = '\0';

    if (!(dir = opendir(newName)))
    {
        free(newName);
        return;
    }

    bool flagged = false;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", newName, entry->d_name);
            GetFiles(path, indent + 2, testing, tbuf);
        }
        char *fullname = malloc(strlen(newName)+strlen(entry->d_name)+2);//+2 null-terminator and /
        if ( fullname == NULL)
        {
            fprintf(stderr,"Not enough Memory\n");
            exit(1);
        }
        strcpy(fullname, newName);
        strcat(fullname, "/");
        strcat(fullname, entry->d_name);
        char *buff = md5sum(fullname);
        if (testing)
            flagged = testscan(buff, tbuf);
        else
            flagged = quickscan(buff);
        strip_extra_spaces(fullname);
        if (isdir(fullname)) {
            continue;
        }
        else if (flagged) {
            fprintf(stderr, "%s    \t\t\t\t\t\t\t\t\t\t\t\t\e[37;1m[ \e[31;1mFLAGGED \e[37;1m]\e[0m\n", fullname);
        }
        else {
            fprintf(stderr, "%s    \t\t\t\t\t\t\t\t\t\t\t\t\e[37;1m[ \e[32;1mOK \e[37;1m] \e[0m\n", fullname);
        }
        free(fullname);
    }
    closedir(dir);
    free(newName);
}
void GetMD5s(char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;
    char *newName;
    newName = strdup(name);
    size_t len = strlen(newName);
    if (newName[len - 1] == '/')
        newName[len - 1] = '\0';
    if (!(dir = opendir(newName)))
    {
        free(newName);
        return;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", newName, entry->d_name);
            GetMD5s(path, indent + 2);
        }
        char *fullname = malloc(strlen(newName)+strlen(entry->d_name)+2);//+2 null-terminator and /
        if ( fullname == NULL)
        {
            fprintf(stderr,"Not enough Memory\n");
            exit(1);
        }
        strcpy(fullname, newName);
        strcat(fullname, "/");
        strcat(fullname, entry->d_name);
        printf("%s\n",md5sum(fullname));
        free(fullname);
    }
    closedir(dir);
    free(newName);
}
