import pymongols


def req_filter(req):
    return True


def res_filter(req, res):
    res.content = 'hello,world'
    res.status = 200


config = {}
config['host'] = '127.0.0.1'
config['port'] = 9090
config['timeout'] = 5000
config['buffer_size'] = 8192
config['thread_size'] = 0
config['max_body_size'] = 4096
config['max_event_size'] = 64


server = pymongols.http_server(config['host'], config['port'], config['timeout'], config['buffer_size'],
                               config['thread_size'], config['max_body_size'], config['max_event_size'])

# server.set_enable_daemon(True)
server.set_enable_multiple_processes(True)
server.set_pidfile("test1.pid")
# server.set_enable_lru_cache(True)
# server.set_lru_cache_expires(1)

server.run(req_filter, res_filter)
