# pymongols
mongols for python

## dependency
- [mongols](https://github.com/webcpp/mongols)
- python3-devel


## install

`cd pymongols && make clean && make && sudo make install`

## usage

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
Time taken for tests:   1.574 seconds
Complete requests:      100000
Failed requests:        0
Keep-Alive requests:    100000
Total transferred:      13600000 bytes
HTML transferred:       1100000 bytes
Requests per second:    63521.67 [#/sec] (mean)
Time per request:       15.743 [ms] (mean)
Time per request:       0.016 [ms] (mean, across all concurrent requests)
Transfer rate:          8436.47 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   1.3      0      23
Processing:     6   15   1.1     15      21
Waiting:        6   15   1.1     15      21
Total:         10   16   1.0     15      32
WARNING: The median and mean for the total time are not within a normal deviation
        These results are probably not that reliable.

Percentage of the requests served within a certain time (ms)
  50%     15
  66%     15
  75%     16
  80%     16
  90%     16
  95%     17
  98%     18
  99%     20
 100%     32 (longest request)


```