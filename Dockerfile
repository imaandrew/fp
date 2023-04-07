FROM debian:testing-slim

RUN apt-get update && \
    apt-get install -y curl gnupg make git && \
    curl https://practicerom.com/public/packages/debian/pgp.pub | apt-key add - && \
    echo deb http://practicerom.com/public/packages/debian staging main > /etc/apt/sources.list.d/practicerom.list && \
    apt-get update && \
    apt-get install -y n64-ultra && \
    git config --global --add safe.directory /src

WORKDIR /src