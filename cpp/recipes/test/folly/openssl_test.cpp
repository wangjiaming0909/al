#include <openssl/ssl.h>
#include <gtest/gtest.h>
#include <curl/curl.h>


TEST(OpenSSl, normal){
    const SSL_METHOD *method = SSLv3_client_method();
    auto bio = BIO_new_fp(stderr, BIO_NOCLOSE);
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();
    ERR_load_CRYPTO_strings();

    auto in = BIO_new(BIO_s_file());
    BIO_set_fp(in, stdin, BIO_NOCLOSE | BIO_FP_TEXT);


    curl_global_init(CURLOPT_SSL_OPTIONS);
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.66.0-DEV");

    auto res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}