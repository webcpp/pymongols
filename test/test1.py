import pymongols


def req_filter(req):
    return True


def res_filter(req, res):
    res.content = 'hello,world\n'
    res.status = 200


config = {'host':'127.0.0.1'
        ,'port':9090
        ,'timeout':5000
        ,'buffer_size':8192
        ,'max_body_size':4096
        ,'max_event_size':64
        }



server = pymongols.http_server(config)

# server.set_enable_daemon(True)
server.set_enable_multiple_processes(True)
server.set_pidfile(__file__+".pid")
# server.set_enable_lru_cache(True)
# server.set_lru_cache_expires(1)

server.run(req_filter, res_filter)
