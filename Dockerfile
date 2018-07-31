FROM rikorose/gcc-cmake:latest

COPY . /project
WORKDIR /project

RUN scripts/build.sh
CMD scripts/test.sh
