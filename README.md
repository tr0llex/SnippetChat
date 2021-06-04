# SnippetChat
[![Build Status](https://travis-ci.com/tr0llex/SnippetChat.svg?branch=main)](https://travis-ci.com/tr0llex/SnippetChat)
[![codecov](https://codecov.io/gh/tr0llex/SnippetChat/branch/main/graph/badge.svg)](https://codecov.io/gh/tr0llex/SnippetChat)

#### Chat for programmers, the main feature of which is sending code snippets and launching them
Technopark Mail.ru 1st semester C++ project

## How to run
```
mkdir build && cd build
cmake ..
make
cd ..
./build/SnippetChat --docroot . --http-address 0.0.0.0 --http-port 8888 --resources-dir=/resources --config ./wt_config.xml
```

### Wt
Загрузка зависимостей:
```
sudo apt-get install gcc g++ libboost-all-dev cmake
```

Загрузка Wt:
```
wget -c https://github.com/emweb/wt/archive/4.5.0.tar.gz
tar xvxf 4.5.0.tar.gz
```

Сборка Wt:
```
cd wt-4.5.0 && mkdir build && cd build
cmake ..
make wt wthttp -j4
sudo make install
ln -s /usr/local/lib/libwt.so /usr/lib/
```

### Cassandra
1)Install cassandra
```
sudo apt install openjdk-8-jre-headless
sudo apt install apt-transport-https
sudo sh -c 'echo "deb http://www.apache.org/dist/cassandra/debian 40x main" > /etc/apt/sources.list.d/cassandra.list'
wget -q -O - https://www.apache.org/dist/cassandra/KEYS | sudo apt-key add -
sudo apt-get update
sudo apt-get install cassandra
```

2)Install cpp driver
```
apt-get install libssl-dev libkrb5-dev zlib1g-dev
wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/dependencies/libuv/v1.35.0/libuv1-dev_1.35.0-1_amd64.deb
wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/dependencies/libuv/v1.35.0/libuv1_1.35.0-1_amd64.deb
wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/cassandra/v2.16.0/cassandra-cpp-driver_2.16.0-1_amd64.deb
wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/cassandra/v2.16.0/cassandra-cpp-driver-dev_2.16.0-1_amd64.deb
wget http://archive.ubuntu.com/ubuntu/pool/main/g/glibc/multiarch-support_2.27-3ubuntu1.2_amd64.deb
sudo dpkg -i multiarch-support_2.27-3ubuntu1.2_amd64.deb
sudo dpkg -i libuv1_1.35.0-1_amd64.deb
sudo dpkg -i libuv1-dev_1.35.0-1_amd64.deb
sudo dpkg -i cassandra-cpp-driver_2.16.0-1_amd64.deb
sudo dpkg -i cassandra-cpp-driver-dev_2.16.0-1_amd64.deb
```

### Tarantool
Загрузить tarantool:
```
curl -L https://tarantool.io/VBBRNxO/release/2.7/installer.sh | bash
sudo apt-get -y install tarantool
```

Проверка порта (если занят, kill):
```
sudo lsof -i :3301
```

Запуск tarantool в терминале:
```
mkdir tarantool_sandbox && cd tarantool_sandbox
tarantool
```

Создание таблиц:
```
box.cfg{listen = 3301}
s = box.schema.space.create('keys')
s = box.schema.space.create('unames', {id = 600})
s:format({{name = 'UserName', type = 'string'},{name = 'token', type = 'string'},{name = 'status', type = 'integer'}})
s:create_index('primary', {type = 'hash', parts = {'UserName'}})
s:create_index('secondary', {type = 'hash', parts = {'token'}})
box.schema.user.grant('guest', 'read,write,execute', 'universe')
```

Если tarantool заснул:
```
tarantool
box.cfg{listen = 3301}
```

### Docker
Добавление репозитория:
```
sudo apt-get update
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg \
    lsb-release 
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
echo \
  "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

Установка Docker Engine:
```
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
```

Управление Docker от non-root пользователя:
```
sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker
sudo chmod 666 /var/run/docker.sock
sudo systemctl restart docker
```

Проверка Docker:
```
docker run hello-world
```
