# GCC + PaaS Web Application

A minimal HTTP web application written in C and compiled using GCC.

## Local build

```bash
gcc -std=c11 -O2 -Wall -Wextra server.c -o server
PORT=10000 ./server
```

Open http://localhost:10000

## PaaS deployment

The project includes a Dockerfile that installs GCC, compiles `server.c`, and runs the compiled binary. It is ready for deployment as a Render Web Service using the Docker runtime.

Render Web Services provide a public `onrender.com` URL. The application listens on `0.0.0.0` and uses the `PORT` environment variable.