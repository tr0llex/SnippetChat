# SnippetChat
[![Build Status](https://travis-ci.com/tr0llex/SnippetChat.svg?branch=main)](https://travis-ci.com/tr0llex/SnippetChat)
[![codecov](https://codecov.io/gh/tr0llex/SnippetChat/branch/main/graph/badge.svg)](https://codecov.io/gh/tr0llex/SnippetChat)

#### Chat for programmers, the main feature of which is sending code snippets and launching them
Technopark Mail.ru 1st semester C++ project


Загрузить турунтул:
curl -L https://tarantool.io/VBBRNxO/release/2.7/installer.sh | bash
sudo apt-get -y install tarantool

После этого в терминале:

mkdir tarantool_sandbox
cd tarantool_sandbox

tarantool

box.cfg{listen = 3301}

<<<<<<< HEAD
s = box.schema.space.create('unames', {id = 600})


s:format({{name = 'UserName', type = 'string'},{name = 'token', type = 'string'},{name = 'status', type = 'integer'}})

s:create_index('primary', {type = 'hash', parts = {'UserName'}})
s:create_index('secondary', {type = 'hash', parts = {'token'}})
=======
s = box.schema.space.create('keys')

s:format({{name = 'id', type = 'integer'},
{name = 'UserName', type = 'string'},
{name = 'token', type = 'string'},
{name = 'status', type = 'integer'}
})

s:create_index('primary', {
         type = 'hash',
         parts = {'id'}
         })
         
 s = box.schema.space.create('keys')
>>>>>>> 9939d1c27647151fa0d33df0114b74e2c0b7b848


s:insert{1, 'admin', 'AAA', 0}
s:insert{2, 'test_1', 'BBA', 1}
s:insert{3, 'test_2', 'BBQ', 1}

=иногда турунтул засыпает, чтобы фиксить надо:

tarantool

box.cfg{listen = 3301}

Замечание:
sudo lsof -i :3301
проверить что порт свободен, нет - kill по занимающим
