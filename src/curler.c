#include "curler.h"
size_t write_data(void *ptr, size_t size, size_t cmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, cmemb, stream);
    return written;
}

static void handle(void *v, double t, double u) {
    if (!v || !t || !u) {
        return;
    }
}

static void barv(double percent) {
    double cc = percent * MAX_BAR_LEN / 100;
    printf("\r[");
    for(int i =0; i < cc; i++) {
        printf("#");
    }
    for(int i=0; i < MAX_BAR_LEN - cc; i++) {
        printf(" ");
    }
    printf("] %.2f%% Complete.",percent);
    fflush(stdout);
}
static int my_progress_func(void *bar,
        double t,
        double d,
        double ultotal,
        double ulnow)
{
    handle(bar,ultotal,ulnow); // just to stop printing damn errors
    barv( d / t* 100);
    return 0;
}

void xcurl(const char *url, const char *of) {
    CURL *curl;
    FILE *fp;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(of, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_func);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode resp = curl_easy_perform(curl);
        if (resp != CURLE_OK) {
            fprintf(stderr,"\nFailed to download db!\n");
            return;
        }
        curl_easy_cleanup(curl);
        fclose(fp);
        fprintf(stderr, "\nDownload complete.\n");
    }
    else {
        fprintf(stderr, "Failed to download to file: %s\n",of);
        return;
    }
}

