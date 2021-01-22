import pymongols


config = {'host':'127.0.0.1'
        ,'port':9090
        ,'timeout':5000
        ,'buffer_size':8192
        ,'max_body_size':4096
        ,'max_event_size':64
        }



server = pymongols.web_server(config)

server.set_root_path("html")
server.set_mime_type_file("html/mime.conf")
server.set_list_directory(True)
server.set_enable_mmap(True)
# server.set_enable_daemon(True)
server.set_enable_multiple_processes(True)
server.set_pidfile(__file__+".pid")

server.run()
