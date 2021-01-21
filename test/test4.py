import pymongols


config = {}
config['host'] = '127.0.0.1'
config['port'] = 9090
config['timeout'] = 5000
config['buffer_size'] = 8192
config['thread_size'] = 0
config['max_body_size'] = 4096
config['max_event_size'] = 64


server = pymongols.web_server(config['host'], config['port'], config['timeout'], config['buffer_size'],
                              config['thread_size'], config['max_body_size'], config['max_event_size'])

server.set_root_path("html")
server.set_mime_type_file("html/mime.conf")
server.set_list_directory(True)
server.set_enable_mmap(True)
# server.set_enable_daemon(True)
server.set_enable_multiple_processes(True)
server.set_pidfile(__file__+".pid")

server.run()
