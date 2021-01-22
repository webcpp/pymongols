from mongols_route import application, template
import os
import sys
import importlib

sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)))

app = application()


@app.route(r'^/(?P<module>.+)/?$',['GET','POST','PUT','DELETE','PATCH'])
def controller(req,res,param):
    try:
        index = importlib.import_module('.index',param['module'].replace("/","."))
        index.handler(req,res,param)
    except Exception as e:
        res.content = repr(e)
        res.status=500




