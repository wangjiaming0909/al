#include <openssl/ssl.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>


TEST(OpenSSl, normal){

static const char* requestMessage ="GET / HTTP/1.1\r\n\
Host: 103.235.46.40:443\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36 OPR/58.0.3135.132\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n";

    SSL_library_init();
    const SSL_METHOD *method = TLSv1_client_method();
    // auto bio = BIO_new_fp(stderr, BIO_NOCLOSE);
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();
    // ERR_load_CRYPTO_strings();

    // auto in = BIO_new(BIO_s_file());
    // BIO_set_fp(in, stdin, BIO_NOCLOSE | BIO_FP_TEXT);
    
    SSL_CTX* ctx = SSL_CTX_new(method);
    if(ctx == nullptr) 
        std::cout << strerror(errno) << std::endl;

    SSL* ssl = SSL_new(ctx);

    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) FAIL();
    sockaddr_in targetAddr;
    ::inet_pton(AF_INET, "103.235.46.40", &targetAddr.sin_addr);
    targetAddr.sin_port = htons(443);
    targetAddr.sin_family = AF_INET;
    SSL_set_fd(ssl, fd);

    ::connect(fd, (sockaddr*)(&targetAddr), sizeof(sockaddr));

    int ret = SSL_connect(ssl);
    if(ret != 1) 
    {
        std::cout << "ssl connect error: " << strerror(errno) << std::endl;
    }

    auto fd_method = BIO_f_ssl();
    auto buf = BIO_new(fd_method);
    auto r = SSL_write(ssl, requestMessage, strlen(requestMessage));
    if (r <= 0)
    {
        int err = SSL_get_error(ssl, r);
        std::cout << "SSL_write error: " << err << std::endl;
        FAIL();
    }
    // BIO_set_ssl(buf, ssl, BIO_NOCLOSE);
    // // BIO_set_fd(buf, fd, BIO_NOCLOSE);
    
    // ret = BIO_write(buf, requestMessage, strlen(requestMessage));
    // if(ret <= 0)
    // {
    //     std::cout << "err bio_put" << strerror(errno) << std::endl;;
    // }

    char* data = static_cast<char*>(::calloc(4096, 1));
    // while(true)
    // {
    //     r = SSL_read(ssl, data, 4096);
    //     int err = SSL_get_error(ssl, r);
    //     if (r <= 0)
    //     {
    //         std::cout << "SSL_read error: " << err << std::endl;
    //         FAIL();
    //     }

    //     std::cout  << data << std::endl;
    //     if (err == SSL_ERROR_WANT_READ)
    //     {
    //         break;
    //     }
    //     memset(data, 0, 4096);
    // }
    free(data);
    std::vector<int> str{};
    auto begin = str.begin();
    auto a = *begin;

    // ret = BIO_read(buf, data, 4096);
    // if(ret <= 0)
    // {
    //     std::cout << "error bio_get: " << strerror(errno) << std::endl;
    // }


    // BIO* mem = BIO_new(BIO_s_mem());
    // BIO_puts(mem, "Hello Wrold\n!");
    // std::cout  << static_cast<char*>(mem->ptr) << std::endl;

    // curl_global_init(CURLOPT_SSL_OPTIONS);
    // CURL* curl = curl_easy_init();

    // curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");

    // curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.66.0-DEV");

    // auto res = curl_easy_perform(curl);

    // curl_easy_cleanup(curl);
    // curl_global_cleanup();
}