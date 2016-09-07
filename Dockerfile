FROM ccvals/ethelos_basic

# Python
RUN apt-get install python

# Java
RUN echo oracle-java8-installer shared/accepted-oracle-license-v1-1 select true | /usr/bin/debconf-set-selections
RUN echo "deb http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee /etc/apt/sources.list.d/webupd8team-java.list
RUN echo "deb-src http://ppa.launchpad.net/webupd8team/java/ubuntu xenial main" | tee -a /etc/apt/sources.list.d/webupd8team-java.list
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys EEA14886
RUN apt-get update && apt-get install oracle-java8-installer oracle-java8-set-default python-pkg-resources python-jinja2 git

# JSHint
RUN npm install -g jshint http-server

# Dependencias para AndroWarn
RUN cd /app/apkmagic/tools/androwarn/deps/chilkat-9.5.0-python-2.7-x86_64-linux && python installChilkat.py

# Volumenes para Android SDK y app
ENV PATH=$PATH:/app/apkmagic
VOLUME ["/app"]
VOLUME ["/sdk"]
