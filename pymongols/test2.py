import pymongols
from mongols_route import application,template

import os

app =application()

@app.route(r'^/test/?$',['GET'])
@app.route(r"^/$",['GET'])
def hello_world(req,res,param):
    res.set_header('Content-Type','text/plain;charset=utf-8')
    res.content='hello,world'
    res.status=200

@app.route(r"^/client/?$",['GET','POST'])
def client(req,res,param):
    res.content='{}<br>{}<br>{}<br>{}<br>{}'.format(req.client,req.method,req.uri,req.user_agent,req.param)
    res.status=200

@app.route(r"^/hello/(?P<who>\w+)?$",['GET'])
def hello(req,res,param):
    res.content='{}={}'.format('who',param['who'])
    res.status=200

@app.route(r'^/template/(?P<name>\w+)/(?P<age>\d+)/?$',['GET'])
def tpl(req,res,param):
    param['title']='测试 jinja2 template'
    tpl_engine = template(os.path.join(os.getcwd(),'python/templates'))
    res.content=tpl_engine.file_render('b.html',param)
    res.status=200


def req_filter(req):
    return True

def res_filter(req,res):
    app.run(req,res)

config={}
config['host']='127.0.0.1'
config['port']=9090
config['timeout']=5000
config['buffer_size']=8192
config['thread_size']=0
config['max_body_size']=4096
config['max_event_size']=64

#server.set_enable_lru_cache(True)
#server.set_lru_cache_expires(1)

server = pymongols.http_server(config['host'],config['port'],config['timeout'],config['buffer_size'],config['thread_size'],config['max_body_size'],config['max_event_size'])


server.run(req_filter,res_filter)