FROM debian:bookworm-slim AS builder
RUN apt-get update && apt-get install -y --no-install-recommends gcc libc6-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY server.c .
RUN gcc -std=c11 -O2 -Wall -Wextra server.c -o server

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /app/server ./server
ENV PORT=10000
EXPOSE 10000
CMD ["/app/server"]