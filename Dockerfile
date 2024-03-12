FROM frolvlad/alpine-gxx:latest 

WORKDIR /c-server

COPY . . 

CMD ["make", "build"]
