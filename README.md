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
./build/SnippetChat --docroot . --http-address 0.0.0.0 --http-port 8080 --resources-dir=/resources
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
