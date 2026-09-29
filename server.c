#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static const char *html_page =
"<!doctype html>"
"<html lang='en'><head><meta charset='utf-8'>"
"<meta name='viewport' content='width=device-width,initial-scale=1'>"
"<title>GCC PaaS Web App</title>"
"<style>body{font-family:Arial,sans-serif;background:#f4f7fb;margin:0;display:grid;place-items:center;height:100vh;color:#1f2937}.card{background:white;max-width:680px;padding:42px;border-radius:18px;box-shadow:0 12px 35px rgba(0,0,0,.10)}h1{margin-top:0;font-size:38px}p{font-size:18px;line-height:1.6}.badge{display:inline-block;padding:8px 12px;border-radius:999px;background:#e8f5e9;color:#1b5e20;font-weight:700}</style>"
"</head><body><main class='card'>"
"<span class='badge'>Deployment successful</span>"
"<h1>Hello from GCC + PaaS</h1>"
"<p>This web application is written in C, compiled with GCC, packaged with Docker, and designed to run as a Render Web Service.</p>"
"<p><strong>Status:</strong> HTTP server is running and ready to serve requests.</p>"
"</main></body></html>";

static int get_port(void) {
    const char *env = getenv("PORT");
    if (!env || !*env) return 10000;
    int p = atoi(env);
    if (p < 1 || p > 65535) return 10000;
    return p;
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)get_port());

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); close(server_fd); return 1;
    }
    if (listen(server_fd, 16) < 0) {
        perror("listen"); close(server_fd); return 1;
    }

    printf("GCC web server listening on 0.0.0.0:%d\n", get_port());
    fflush(stdout);

    for (;;) {
        int client = accept(server_fd, NULL, NULL);
        if (client < 0) continue;

        char request[2048];
        (void)read(client, request, sizeof(request) - 1);

        char header[256];
        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n"
                 "Content-Length: %zu\r\n"
                 "Connection: close\r\n\r\n", strlen(html_page));

        (void)write(client, header, strlen(header));
        (void)write(client, html_page, strlen(html_page));
        close(client);
    }

    close(server_fd);
    return 0;
}