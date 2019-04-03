# pymongols
mongols for python

## dependency
- [mongols](https://github.com/webcpp/mongols)
- python2,3 devel


## install

`cd pymongols && make clean && make && sudo make install`

## usage

see `test`

## hello world

```python

import pymongols


def req_filter(req):
    return True

def res_filter(req,res):
    res.content='hello,world'
    res.status=200

config={}
config['host']='127.0.0.1'
config['port']=9090
config['timeout']=5000
config['buffer_size']=8192
config['thread_size']=0
config['max_body_size']=4096
config['max_event_size']=64


server = pymongols.http_server(config['host'],config['port'],config['timeout'],config['buffer_size'],config['thread_size'],config['max_body_size'],config['max_event_size'])

#server.set_enable_lru_cache(True)
#server.set_lru_cache_expires(1)


server.run(req_filter,res_filter)


```

## benchmark

```txt

Server Software:        mongols/1.6.8
Server Hostname:        localhost
Server Port:            9090

Document Path:          /
Document Length:        11 bytes

Concurrency Level:      1000
Time taken for tests:   0.898 seconds
Complete requests:      100000
Failed requests:        0
Keep-Alive requests:    100000
Total transferred:      13600000 bytes
HTML transferred:       1100000 bytes
Requests per second:    111341.22 [#/sec] (mean)
Time per request:       8.981 [ms] (mean)
Time per request:       0.009 [ms] (mean, across all concurrent requests)
Transfer rate:          14787.51 [Kbytes/sec] received

```