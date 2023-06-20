#include <stdio.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <unistd.h>

#define ARR_LEN(A) ({(sizeof((A)) / sizeof((A)[0]));})
const int PROGRESS_BAR_LENGTH = 30;

typedef struct {
    long total_bytes;
} status_info;

void update_bar(int percent_done, status_info *status) {
    int num_char = percent_done * PROGRESS_BAR_LENGTH / 100;
    printf("\r[");
    for (int i=0; i < num_char; i++) {
        printf("#");
    }
    for (int i=0; i < PROGRESS_BAR_LENGTH - num_char; i++) {
        printf("x");
    }
    printf("] %d%% Done (%ld MB)", percent_done, status->total_bytes / 1000000);
    fflush(stdout);
}

size_t got_data(char *buffer, size_t item_size, size_t num_items, void *status_inf) {
    status_info *status = status_inf;

    size_t bytes = item_size * num_items;
    status->total_bytes += (long) bytes;

    return bytes;
}

bool download_url(char *url, status_info *status_inf) {
    CURL *curl = curl_easy_init();
    if (!curl) return false;

    // set options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // launch this function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);
    // pass this struct to the got_data function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, status_inf);
    // follow redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    // do the download
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK) return false;
    return true;
}


int main() {

    char *urls[] = {
            "https://www.mongodb.com/docs/manual/tutorial/query-documents/",
            "https://www.alza.sk/samsung-galaxy-s23/18902477.htm",
            "https://dennikn.sk/",
            "https://en.wikipedia.org/wiki/MCOLN1",
            "https://www.youtube.com/watch?v=t_vM_8TLjFE",
            "https://gnomad.broadinstitute.org/downloads"
    };

    int urls_size = ARR_LEN(urls);
    status_info info;
    info.total_bytes = 0;


    update_bar(0, &info);
    for (int i=0; i < urls_size; i++) {
        download_url(urls[i], &info);
        update_bar((i+1)*100/urls_size, &info);
    }
    printf("\n");


    return 0;
}
