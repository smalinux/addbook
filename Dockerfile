FROM ubuntu
LABEL Description="AddBook Description"

ENV HOME /root

# Packages or dep
RUN apt-get update && apt-get -y --no-install-recommends install cmake build-essential libsqlite3-dev sqlite3

# COPY the files from OS
COPY * ./addbook

# Commands
CMD mkdir addbook; cd addbook; make; ./addbook
