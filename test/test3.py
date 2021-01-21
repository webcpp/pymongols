import pymongols


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
server.set_pidfile(__file__+".pid")
# server.set_enable_lru_cache(True)
# server.set_lru_cache_expires(1)
server.set_enable_session(True)


def req_filter(req):
    return True


def default_handler(req, res, param):
    res.set_header('Content-Type', 'text/plain;charset=utf-8')
    res.content = 'hello,world'
    res.status = 200


def get_handler(req, res, param):
    res.set_header('test', 'pymongols')
    res.content = req.method+'<br />'+param[1]
    res.status = 200


def post_handler(req, res, param):
    get_handler(req, res, param)


server.add_route(['GET', 'POST'], r"^/$", default_handler)
#server.add_route(['GET'], r"^/get/([a-zA-Z]+)/?$", get_handler)
#server.add_route(['POST'], r"^/post/([0-9]+)/?$", post_handler)


server.run_with_route(req_filter)
