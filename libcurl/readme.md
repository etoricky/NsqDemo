1. Run "x64 Native Tools Command Prompt"
2. Download curl-7.60.0 from https://curl.haxx.se/download.html
3. Unzip and cd to winbuild
4. nmake /f Makefile.vc mode=static VC=14 ENABLE_IPV6=no MACHINE=AMD64
5. cd to builds/libcurl-vc14-AMD64-release-static-sspi-winssl
6. Copy "include" and "libcurl_a.lib" to Visual Studio project
7. Refer to https://curl.haxx.se/libcurl/c/resolve.html
8. Modify to below code

    #include <stdio.h>
    #include "include/curl/curl.h"
    int main(void) {
        CURLcode res = CURLE_OK;
        CURL *curl = curl_easy_init();
        if (curl) {
            const char *data = "data to send";
            curl_easy_setopt(curl, CURLOPT_URL, "http://172.31.118.249:4151/pub?topic=test");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 12L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return res;
    }